#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <magica/uint256.h>
#include <magica/decimal_private_def.h>
#include <magica/decimal_private_impl.h>
#include <magica/decimal.h>

static mg_decimal_error __mg_decimal_divide_impl(
		const mg_uint256 *_fraction1,
		const mg_uint256 *_fraction2,
		/*inout*/int *_scale,
		/*inout*/mg_uint256 *_q);

MG_DECIMAL_API mg_decimal_error mg_decimal_divide(const mg_decimal *op1, const mg_decimal *op2, /*out*/mg_decimal *ret)
{
	mg_decimal_error err;
	int ierr;
	int sign, sign1, sign2;
	int scale, scale1, scale2;
	int status1, status2;
	mg_uint256 buf1, buf2, buf3, buf4;
	mg_uint256 *fraction1 = &buf1, *fraction2 = &buf2, *q = &buf3, *tmp = &buf4;

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
				mg_decimal_nan(ret);
				err = MG_DECIMAL_ERROR_ZERODIVIDE;
				goto _ERROR;
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
			mg_decimal_nan(ret);
			err = MG_DECIMAL_ERROR_NAN;
			goto _ERROR;
		}
	}

	if (mg_uint256_is_zero(fraction2)) {
		mg_decimal_nan(ret);
		err = MG_DECIMAL_ERROR_ZERODIVIDE;
		goto _ERROR;
	}

	if (sign1 != sign2)
		sign = SIGN_NEGATIVE;
	else
		sign = SIGN_POSITIVE;

	scale = scale1 - scale2;

	ierr = mg_uint256_div_1(/*inout*/fraction1, fraction2, /*out*/q);
	assert(ierr == 0);

	err = __mg_decimal_divide_impl(fraction1, fraction2, /*inout*/&scale, /*inout*/q);
	if(err != 0)
		goto _ERROR;

	if(scale > 0) {
		mg_uint256_mul128(q, mg_uint256_get_10eN(scale), /*out*/tmp);
		scale = 0;
		if(__mg_decimal_is_overflow(tmp)) {
			err = MG_DECIMAL_ERROR_OVERFLOW;
			goto _ERROR;
		}
		err = __mg_decimal_set_1(ret, sign, scale, tmp);
		if(err != 0)
			goto _ERROR;
	} else {
		err = __mg_decimal_set_1(ret, sign, scale, q);
		if(err != 0)
			goto _ERROR;
	}

_EXIT:
	return 0;
_ERROR:
	return err;
}

static mg_decimal_error __mg_decimal_divide_impl(
		const mg_uint256 *_fraction1, 
		const mg_uint256 *_fraction2, 
		/*inout*/int *_scale,
		/*inout*/mg_uint256 *_q)
{
	mg_decimal_error err;
	int ierr;
	int overflow;
	mg_uint256 buf1, buf2, buf3, buf4, buf5;
	mg_uint256 *fraction1 = &buf1;
	mg_uint256 *fraction2 = &buf2;
	mg_uint256 *q = &buf3;
	mg_uint256 *tmp = &buf4;
	mg_uint256 *tmp2 = &buf5;
	
	int scale = *_scale;

	const mg_uint256 *v10e18 = mg_uint256_get_10eN(18);
	
	*fraction1 = *_fraction1;
	*fraction2 = *_fraction2;

	*q = *_q;

	if(!mg_uint256_is_zero(fraction1)) {
		int baseDigits = 0;
		while(!mg_uint256_is_zero(fraction1)) {
			if(scale - baseDigits < SCALE_MIN)
				break;
			int digits = mg_uint256_get_digits(q);
			if(digits > DIGIT_MAX)
				break;

			// fraction1 = fraction1 * 10^18
			overflow = mg_uint256_mul_digits(
					fraction1, MG_UINT256_DIGITS_256, v10e18, MG_UINT256_DIGITS_64, /*out*/tmp);
			assert(overflow == 0);
			mg_uint256_swap(&fraction1, &tmp);

			// q = q * 10^18
			overflow = mg_uint256_mul_digits(
					q, MG_UINT256_DIGITS_256, v10e18, MG_UINT256_DIGITS_64, /*out*/tmp);
			assert(overflow == 0);
			mg_uint256_swap(&q, &tmp);

			// q = q + fraction1 / fraction2;
			// fraction1 = fraction1 % fraction2;
			ierr = mg_uint256_div_1(/*inout*/fraction1, fraction2, /*out*/tmp);
			assert(ierr == 0);
			mg_uint256_add_1(/*inout*/q, tmp);

			baseDigits += 18;
		}
		scale -= baseDigits;

		int rounded_scale;
		err = __mg_decimal_round_down_max_digits(q, scale, /*out*/&rounded_scale);
		if(err != 0)
			goto _ERROR;
		scale = rounded_scale;
	}
	
	*_scale = scale;
	*_q = *q;
	
	return 0;
_ERROR:
	return err;
}
