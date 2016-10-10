#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <magica/uint256.h>
#include <magica/decimal_private_def.h>
#include <magica/decimal_private_impl.h>
#include <magica/decimal.h>

MG_DECIMAL_API mg_decimal_error mg_decimal_min(const mg_decimal *value1, const mg_decimal *value2, /*out*/mg_decimal *ret)
{
	mg_decimal_error err;
	int ret1;

	err = mg_decimal_compare(value1, value2, /*out*/&ret1);
	if(err != 0)
		goto _ERROR;

	if(ret1 > 0) {
		*ret = *value2;
	} else {
		*ret = *value1;
	}

	return 0;
_ERROR:
	return err;
}
