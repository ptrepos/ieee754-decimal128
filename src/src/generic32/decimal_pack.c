#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <magica/uint256.h>
#include <magica/decimal_private_def.h>
#include <magica/decimal_private_impl.h>
#include <magica/decimal.h>

MG_DECIMAL_API void mg_decimal_pack(const mg_decimal *value, /*out*/uint64_t *low, /*out*/uint64_t *high)
{
	*low = (uint64_t)value->w[0] | ((uint64_t)value->w[1] << 32);
	*high = (uint64_t)value->w[2] | ((uint64_t)value->w[3] << 32);
}
