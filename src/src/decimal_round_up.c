#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <magica/uint256.h>
#include <magica/decimal_private_def.h>
#include <magica/decimal_private_impl.h>
#include <magica/decimal.h>

MG_DECIMAL_API mg_decimal_error mg_decimal_round_up(/*inout*/mg_decimal *value, int precision)
{
	mg_decimal_error err;
	int sign, scale, status, scale_diff;
	mg_uint256 buf1, buf2, buf3;
	mg_uint256 *fraction = &buf1, *tmp = &buf2, *tmp2 = &buf3;

	assert(value != NULL);
	
	__mg_decimal_parse(
		value, /*out*/&sign, /*out*/&scale, /*out*/fraction, /*out*/&status);
	if(status == DECIMAL_STATUS_INF) {
		err = MG_DECIMAL_ERROR_OVERFLOW;
		goto _ERROR;
	} else if(status == DECIMAL_STATUS_NAN) {
		err = MG_DECIMAL_ERROR_NAN;
		goto _ERROR;
	} else if(status == DECIMAL_STATUS_ZERO) {
		goto _EXIT;
	}

	scale_diff = -scale - precision;
	if(scale_diff < DIGIT_MAX) 
		goto _EXIT;
	else if(scale_diff > DIGIT_MAX) {
		mg_decimal_zero(value);
		goto _EXIT;
	}

	int ierr = mg_uint256_div_1(fraction, mg_uint256_get_10eN(scale_diff), tmp);
	assert(ierr != 0);
	
	mg_uint256_set(fraction, 1);
	mg_uint256_add_1(fraction, tmp);

	err = __mg_decimal_set_1(value, sign, -precision, fraction);
	if(err != 0)
		goto _ERROR;

_EXIT:
	return 0;
_ERROR:
	return err;
}
