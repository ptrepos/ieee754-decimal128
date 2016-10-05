#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <magica/uint256.h>
#include <magica/decimal_private_def.h>
#include <magica/decimal_private_impl.h>
#include <magica/decimal.h>

MG_DECIMAL_API mg_decimal_error mg_decimal_value_of_uint64(uint64_t value, /*out*/mg_decimal *ret)
{
	mg_decimal_error err;
	mg_uint256 fraction;

	mg_uint256_set(&fraction, value);

	err = __mg_decimal_set_1(ret, SIGN_POSITIVE, 0, &fraction);
	assert(err == 0);

	return 0;
}
