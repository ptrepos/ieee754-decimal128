#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <magica/uint256.h>
#include <magica/decimal_private_def.h>
#include <magica/decimal_private_impl.h>
#include <magica/decimal.h>

MG_DECIMAL_API mg_decimal_error mg_decimal_to_float(const mg_decimal *value, /*out*/float *ret)
{
	mg_decimal_error err;
	double fp;
	
	err = mg_decimal_to_double(value, /*out*/&fp);
	if(err != 0)
		goto _ERROR;
	
	*ret = (float)fp;
	
	return 0;
_ERROR:
	return err;
}
