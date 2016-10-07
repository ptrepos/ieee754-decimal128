#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <magica/uint256.h>
#include <magica/decimal_private_def.h>
#include <magica/decimal_private_impl.h>
#include <magica/decimal.h>

MG_DECIMAL_API mg_decimal_error mg_decimal_value_of_double(double value, /*out*/mg_decimal *ret)
{
	char buf[256];
	
	snprintf(buf, 256, "%.34g", value);

	return mg_decimal_parse_string(buf, /*out*/ret);
}
