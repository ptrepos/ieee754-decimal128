#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <magica/uint256.h>
#include <magica/decimal_private_def.h>
#include <magica/decimal_private_impl.h>
#include <magica/decimal.h>

static mg_decimal_error __mg_decimal_round_zero_digits(
		mg_uint256 *value, 
		int scale, 
		/*out*/int *rounded_scale);

MG_DECIMAL_API mg_decimal_error mg_decimal_normalize(/*inout*/mg_decimal *value)
{
	mg_decimal_error err;
	int sign, scale, status, rounded_scale;
	mg_uint256 buf1;
	mg_uint256 *fraction = &buf1;

	__mg_decimal_parse(
		value, /*out*/&sign, /*out*/&scale, /*out*/fraction, /*out*/&status);
	if(status == DECIMAL_STATUS_NAN) {
		err = MG_DECIMAL_ERROR_NAN;
		goto _ERROR;
	} else if(status == DECIMAL_STATUS_INF) {
		err = MG_DECIMAL_ERROR_OVERFLOW;
		goto _ERROR;
	} else if(status == DECIMAL_STATUS_ZERO) {
		goto _EXIT;
	}

	err = __mg_decimal_round_zero_digits(fraction, scale, /*out*/&rounded_scale);
	if (err != 0)
		goto _ERROR;
	scale = rounded_scale;

	if (mg_uint256_is_zero(fraction)) {
		sign = 0;
		scale = 0;
	}

	err = __mg_decimal_set_1(value, sign, scale, fraction);
	if(err != 0)
		goto _ERROR;

_EXIT:
	return 0;
_ERROR:
	return err;
}


static int get_roundable_zero_digits(uint64_t value, uint64_t *cutted)
{
	int shiftDigits;

	shiftDigits = 0;

	if (value % 100000000L == 0) {
		shiftDigits += 8;
		value /= 100000000L;

		if (value % 100000000L == 0) {
			shiftDigits += 8;
			value /= 100000000L;
		}
	}
	if (value % 10000L == 0) {
		shiftDigits += 4;
		value /= 10000L;
	}
	if (value % 100L == 0) {
		shiftDigits += 2;
		value /= 100L;
	}
	if (value % 10L == 0) {
		shiftDigits += 1;
		value /= 10L;
	}

	*cutted = value;

	return shiftDigits;
}

static mg_decimal_error __mg_decimal_round_zero_digits(mg_uint256 *value, int scale, /*out*/int *rounded_scale)
{
	//mg_decimal_error err;
	mg_uint256 buf1, buf2;
	mg_uint256  *work = &buf1, *tmp = &buf2;
	const mg_uint256 *shift;
	uint64_t v;
	int vShift;
	int cutted;

	assert(value != NULL);

	*work = *value;

	cutted = 0;

	// è¨êîì_à»â∫ÇÃâ∫à 0åÖçÌèú
	while (scale < 0) {
		int nShift = 18;
		if (-scale < nShift)
			nShift = -scale;
		int ierr = mg_uint256_div_1(work, mg_uint256_get_10eN(nShift), tmp);
		assert(ierr != 0);

		if (mg_uint256_is_zero(work)) {
			mg_uint256_swap(&work, &tmp);
			scale += nShift;
			cutted += nShift;
		} else {
			v = mg_uint256_get_uint64(work);
			vShift = get_roundable_zero_digits(v, &v);
			if (vShift <= 0)
				break;

			shift = mg_uint256_get_10eN(nShift - vShift);

			mg_uint256_mul128(tmp, shift, work);

			mg_uint256_set(tmp, v);
			mg_uint256_add_1(work, tmp);

			scale += vShift;
			cutted += vShift;

			break;
		}
	}

	if(cutted > 0)
		*value = *work;

	*rounded_scale = scale;

	return 0;
//_ERROR:
//	return err;
}
