#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <magica/uint256.h>
#include <magica/decimal_private_def.h>
#include <magica/decimal_private_impl.h>
#include <magica/decimal.h>

MG_DECIMAL_API mg_decimal_error mg_decimal_negate(mg_decimal *op1)
{
	if(mg_decimal_is_zero(op1))
		return 0;

	__mg_decimal_set_sign(op1, 1 - __mg_decimal_get_sign(op1));

	return 0;
}
