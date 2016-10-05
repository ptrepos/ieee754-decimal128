#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <magica/uint256.h>
#include <magica/decimal_private_def.h>
#include <magica/decimal_private_impl.h>
#include <magica/decimal.h>

MG_DECIMAL_API mg_decimal_error mg_decimal_abs(const mg_decimal *value, /*out*/mg_decimal *ret)
{
	*ret = *value;

	__mg_decimal_set_sign(ret, 0);
	
	return 0;
}
