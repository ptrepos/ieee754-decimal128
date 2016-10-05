#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <memory.h>
#include <magica/uint256.h>
#include <magica/decimal_private_def.h>
#include <magica/decimal_private_impl.h>
#include <magica/decimal.h>

MG_DECIMAL_API void mg_decimal_infinity(/*out*/mg_decimal *value, bool is_positive)
{
	mg_uint256 fraction = { 0 };

	__mg_decimal_set(
			/*out*/value, 
			is_positive ? SIGN_POSITIVE: SIGN_NEGATIVE,
			SCALE_INFINITY, 
			&fraction);
}
