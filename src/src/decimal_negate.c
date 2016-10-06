#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <magica/uint256.h>
#include <magica/decimal_private_def.h>
#include <magica/decimal_private_impl.h>
#include <magica/decimal.h>

MG_DECIMAL_API mg_decimal_error mg_decimal_negate(const mg_decimal *op1, /*out*/mg_decimal *ret)
{
	if(mg_decimal_is_zero(op1))
		return 0;
	
	*ret = *op1;
	__mg_decimal_set_sign(/*out*/ret, 1 - __mg_decimal_get_sign(ret));

	return 0;
}
