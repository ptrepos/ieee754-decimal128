#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <magica/uint256.h>
#include <magica/decimal_private_def.h>
#include <magica/decimal_private_impl.h>
#include <magica/decimal.h>

MG_DECIMAL_API mg_decimal_error mg_decimal_add(const mg_decimal *op1, const mg_decimal *op2, /*out*/mg_decimal *ret)
{
	mg_decimal_error err;
	int sign, sign1, sign2;
	int scale, scaleDiff, scale1, scale2;
	int status1, status2;
	mg_uint256 buf1, buf2, buf3;
	mg_uint256 *fraction1 = &buf1, *fraction2 = &buf2, *tmp = &buf3;

	assert(ret != NULL);

	__mg_decimal_parse(
		op1, /*out*/&sign1, /*out*/&scale1, /*out*/fraction1, /*out*/&status1);
	__mg_decimal_parse(
		op2, /*out*/&sign2, /*out*/&scale2, /*out*/fraction2, /*out*/&status2);

	if(status1 == DECIMAL_STATUS_NAN || status2 == DECIMAL_STATUS_NAN) {
		mg_decimal_nan(/*out*/ret);
		err = MG_DECIMAL_ERROR_NAN;
		goto _ERROR;
	}
	if(status1 == DECIMAL_STATUS_INF || status2 == DECIMAL_STATUS_INF) {
		mg_decimal_infinity(/*out*/ret, sign1 == SIGN_POSITIVE);
		err = MG_DECIMAL_ERROR_OVERFLOW;
		goto _ERROR;
	}
	if(status1 == DECIMAL_STATUS_ZERO) {
		*ret = *op2;
		goto _EXIT;
	} else if(status2 == DECIMAL_STATUS_ZERO) {
		*ret = *op1;
		goto _EXIT;
	}

	scaleDiff = scale1 - scale2;
	if (scaleDiff == 0) {
		scale = scale1;

		if (sign1 == sign2) {
			mg_uint256_add128_1(fraction1, fraction2);
			sign = sign1;
		} else {
			int borrow = mg_uint256_sub128_1(fraction1, fraction2);
			if (borrow == 0)
				sign = sign1;
			else {
				mg_uint256_neg128_1(fraction1);
				sign = sign1 == SIGN_POSITIVE ? SIGN_NEGATIVE : SIGN_POSITIVE;
			}
		}
	} else {
		if (scaleDiff > DIGIT_MAX) {
			*ret = *op1;
			goto _EXIT;
		} else if(scaleDiff < -DIGIT_MAX) {
			*ret = *op2;
			goto _EXIT;
		} else if (scaleDiff > 0) {
			scale = scale2;

			mg_uint256_mul128(fraction1, mg_uint256_get_10eN(scaleDiff), tmp);
			mg_uint256_swap(&fraction1, &tmp);
		} else {
			scale = scale1;

			mg_uint256_mul128(fraction2, mg_uint256_get_10eN(-scaleDiff), tmp);
			mg_uint256_swap(&fraction2, &tmp);
		}

		if (sign1 == sign2) {
			mg_uint256_add_1(/*inout*/fraction1, fraction2);
			sign = sign1;
		} else {
			int borrow = mg_uint256_sub_1(/*inout*/fraction1, fraction2);
			if (borrow == 0)
				sign = sign1;
			else {
				mg_uint256_neg_1(/*inout*/fraction1);
				sign = sign1 == SIGN_POSITIVE ? SIGN_NEGATIVE : SIGN_POSITIVE;
			}
		}
	}

	if(__mg_decimal_is_overflow(fraction1)) {
		int rounded_scale;
		err = __mg_decimal_round_down_max_digits(/*inout*/fraction1, scale, /*out*/&rounded_scale);
		if(err != 0)
			goto _ERROR;
		scale = rounded_scale;

		if(__mg_decimal_is_overflow(fraction1)) {
			err = MG_DECIMAL_ERROR_OVERFLOW;
			goto _ERROR;
		}
	}

	err = __mg_decimal_set_1(ret, sign, scale, fraction1);
	if(err != 0)
		goto _ERROR;

_EXIT:
	return 0;
_ERROR:
	return err;
}
