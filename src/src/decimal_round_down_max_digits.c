#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <magica/uint256.h>
#include <magica/decimal_private_def.h>
#include <magica/decimal_private_impl.h>
#include <magica/decimal.h>

MG_PRIVATE mg_decimal_error __mg_decimal_round_down_max_digits(mg_uint256 *value, int scale, int *rounded_scale)
{
	mg_decimal_error err;

	int digits = mg_uint256_get_digits(value);
	if(digits >= DIGIT_MAX) {
		mg_uint256 tmp, dummy;
		int ierr = mg_uint256_div(
				value, 
				mg_uint256_get_10eN(digits - DIGIT_MAX),
				/*out*/&tmp,
				&dummy);
		if(ierr != 0) {
			err = MG_DECIMAL_ERROR_ZERODIVIDE;
			goto _ERROR;
		}
		
		*value = tmp;
		*rounded_scale = scale + (digits - DIGIT_MAX);
	} else {
		*rounded_scale = scale;
	}

	return 0;
_ERROR:
	return err;
}
