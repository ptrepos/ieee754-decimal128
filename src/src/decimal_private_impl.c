#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <magica/uint256.h>
#include <magica/decimal_private_def.h>
#include <magica/decimal_private_impl.h>
#include <magica/decimal.h>

MG_PRIVATE mg_decimal_error adjust_overflow_scale(
		int sign, 
		int scale, 
		const mg_uint256 *fraction,
		/*out*/int *adjusted_scale,
		/*out*/mg_uint256 *adjusted_fraction,
		/*out*/mg_decimal *value)
{
	mg_decimal_error err;

	if(scale - SCALE_MAX < DIGIT_MAX) {
		mg_uint256_mul128(fraction, mg_uint256_get_10eN(scale - SCALE_MAX), adjusted_fraction);

		if(__mg_decimal_is_overflow(adjusted_fraction) != 0) {
			mg_decimal_infinity(/*out*/value, sign == SIGN_POSITIVE);
			err = MG_DECIMAL_ERROR_OVERFLOW;
			goto _ERROR;
		}

		*adjusted_scale = SCALE_MAX;
	} else {
		mg_decimal_infinity(/*out*/value, sign == SIGN_POSITIVE);
		err = MG_DECIMAL_ERROR_OVERFLOW;
		goto _ERROR;
	}

	return 0;
_ERROR:
	return err;
}
