#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <magica/uint256.h>
#include <magica/decimal_private_def.h>
#include <magica/decimal_private_impl.h>
#include <magica/decimal.h>

MG_DECIMAL_API mg_decimal_error mg_decimal_multiply(const mg_decimal *op1, const mg_decimal *op2, /*out*/mg_decimal *ret)
{
	mg_decimal_error err;
	int sign, sign1, sign2;
	int scale, scale1, scale2;
	int status1, status2;
	mg_uint256 buf1, buf2, buf3;
	mg_uint256 *fraction1 = &buf1, *fraction2 = &buf2, *fraction = &buf3;

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
		if(status1 == DECIMAL_STATUS_INF || status2 != DECIMAL_STATUS_INF) {
			if(mg_uint256_is_zero(fraction2)) {
				mg_decimal_zero(/*out*/ret);
				goto _EXIT;
			} else {
				mg_decimal_infinity(/*out*/ret, sign1 == sign2);
				err = MG_DECIMAL_ERROR_OVERFLOW;
				goto _ERROR;
			}
		} else if(status1 != DECIMAL_STATUS_INF || status2 == DECIMAL_STATUS_INF) {
			if(mg_uint256_is_zero(fraction1)) {
				mg_decimal_zero(/*out*/ret);
				goto _EXIT;
			} else {
				mg_decimal_infinity(/*out*/ret, sign1 == sign2);
				err = MG_DECIMAL_ERROR_OVERFLOW;
				goto _ERROR;
			}
		} else {
			mg_decimal_infinity(/*out*/ret, sign1 == sign2);
			err = MG_DECIMAL_ERROR_OVERFLOW;
			goto _ERROR;
		}
	}

	if(status1 == DECIMAL_STATUS_ZERO || status2 == DECIMAL_STATUS_ZERO) {
		mg_decimal_zero(/*out*/ret);
		goto _EXIT;
	}

	if (sign1 != sign2)
		sign = SIGN_NEGATIVE;
	else
		sign = SIGN_POSITIVE;

	scale = scale1 + scale2;

	mg_uint256_mul128(fraction1, fraction2, fraction);

	if (__mg_decimal_is_overflow(fraction)) {
		int rounded_scale;
		err = __mg_decimal_round_down_max_digits(fraction, scale, /*out*/&rounded_scale);
		if(err != 0)
			goto _ERROR;
		scale = rounded_scale;
	}

	err = __mg_decimal_set_1(ret, sign, scale, fraction);
	if(err != 0)
		goto _ERROR;

_EXIT:
	return 0;
_ERROR:
	return err;
}
