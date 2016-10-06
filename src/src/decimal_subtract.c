#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <magica/uint256.h>
#include <magica/decimal_private_def.h>
#include <magica/decimal_private_impl.h>
#include <magica/decimal.h>

MG_DECIMAL_API mg_decimal_error mg_decimal_subtract(const mg_decimal *op1, const mg_decimal *op2, /*out*/mg_decimal *ret)
{
	mg_decimal_error err;

	assert(ret != NULL);

	mg_decimal nagated_op2 = *op2;
	err = mg_decimal_negate(&nagated_op2, /*out*/&nagated_op2);
	if(err != 0)
		goto _ERROR;
	err = mg_decimal_add(op1, &nagated_op2, ret);
	if(err != 0)
		goto _ERROR;

	return 0;
_ERROR:
	return err;
}
