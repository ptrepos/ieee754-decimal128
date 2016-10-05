#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <memory.h>
#include <magica/uint256.h>
#include <magica/decimal_private_def.h>
#include <magica/decimal_private_impl.h>
#include <magica/decimal.h>

MG_DECIMAL_API bool mg_decimal_is_nan(const mg_decimal *op1)
{
	int sign, scale, status;
	mg_uint256 fraction;

	__mg_decimal_parse(op1, /*out*/&sign, /*out*/&scale, /*out*/&fraction, /*out*/&status);

	return status == DECIMAL_STATUS_NAN;
}
