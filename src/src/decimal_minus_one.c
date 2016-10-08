#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <memory.h>
#include <magica/uint256.h>
#include <magica/decimal_private_def.h>
#include <magica/decimal_private_impl.h>
#include <magica/decimal.h>

MG_DECIMAL_API void mg_decimal_minus_one(/*out*/mg_decimal *value)
{
	mg_decimal_unpack(0x0000000000000001ULL, 0xb040000000000000ULL, /*out*/value);
}
