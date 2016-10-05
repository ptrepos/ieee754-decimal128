#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <magica/uint256.h>
#include <magica/decimal_private_def.h>
#include <magica/decimal_private_impl.h>
#include <magica/decimal.h>

MG_DECIMAL_API mg_decimal_error mg_decimal_to_uint(const mg_decimal *value, /*out*/unsigned int *ret)
{
	mg_decimal_error err;
	int sign, scale, status;
	mg_uint256 buf1, buf2;
	mg_uint256 *fraction = &buf1, *tmp = &buf2;

	__mg_decimal_parse(
		value, /*out*/&sign, /*out*/&scale, /*out*/fraction, /*out*/&status);
	if(status == DECIMAL_STATUS_INF) {
		err = MG_DECIMAL_ERROR_OVERFLOW;
		goto _ERROR;
	} else if(status == DECIMAL_STATUS_NAN) {
		err = MG_DECIMAL_ERROR_NAN;
		goto _ERROR;
	} else if(status == DECIMAL_STATUS_ZERO) {
		*ret = 0;
		goto _EXIT;
	}

	if(sign == SIGN_NEGATIVE) {
		err = MG_DECIMAL_ERROR_OVERFLOW;
		goto _ERROR;
	}
	if(scale < -DIGIT_MAX) {
		err = MG_DECIMAL_ERROR_OVERFLOW;
		goto _ERROR;
	}
	if(scale > DIGIT_MAX) {
		err = MG_DECIMAL_ERROR_OVERFLOW;
		goto _ERROR;
	}

	if (scale < 0) {
		int ierr = mg_uint256_div_1(/*inout*/fraction, mg_uint256_get_10eN(-scale), /*out*/tmp);
		assert(ierr == 0);
		mg_uint256_swap(/*inout*/&fraction, /*inout*/&tmp);
	} else if(scale > 0) {
		mg_uint256_mul128(fraction, mg_uint256_get_10eN(scale), /*out*/tmp);
		mg_uint256_swap(/*inout*/&fraction, /*inout*/&tmp);
	}
	
	assert(sign == SIGN_POSITIVE);

	mg_uint256_set(/*out*/tmp, UINT32_MAX);

	// out of int64.
	if (mg_uint256_compare(fraction, tmp) > 0) {
		err = MG_DECIMAL_ERROR_OVERFLOW;
		goto _ERROR;
	}

	unsigned int work = (unsigned int)mg_uint256_get_uint64(fraction);
	*ret = work;

_EXIT:
	return 0;
_ERROR:
	return err;
}
