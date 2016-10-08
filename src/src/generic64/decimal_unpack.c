#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <magica/uint256.h>
#include <magica/decimal_private_def.h>
#include <magica/decimal_private_impl.h>
#include <magica/decimal.h>

MG_DECIMAL_API void mg_decimal_unpack(uint64_t low, uint64_t high, /*out*/mg_decimal *ret)
{
	ret->w[0] = low;
	ret->w[1] = high;
}
