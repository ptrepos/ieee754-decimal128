#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <magica/uint256.h>
#include <magica/decimal_private_def.h>
#include <magica/decimal_private_impl.h>
#include <magica/decimal.h>

MG_DECIMAL_API int mg_decimal_compare_1(const mg_decimal *op1, const mg_decimal *op2)
{
	int ret;
	mg_decimal_error err = mg_decimal_compare(op1, op2, /*out*/&ret);

	if(err != 0)
		abort();

	return ret;
}
