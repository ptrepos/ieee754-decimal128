#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <memory.h>
#include <magica/uint256.h>
#include <magica/decimal_private_def.h>
#include <magica/decimal_private_impl.h>
#include <magica/decimal.h>

MG_DECIMAL_API void mg_decimal_max_value(/*out*/mg_decimal *value)
{
	mg_decimal_unpack(0x378d8e63ffffffffULL, 0x6001ed09bead87c0ULL, /*out*/value);
}
