#include <magica/test.h>
#include <magica/decimal.h>

void test_decimal_max_value()
{
	MG_TEST_BEGIN();
	
	mg_decimal value;
	
	mg_decimal_max_value(/*out*/&value);
	
	MG_ASSERT(mg_decimal_is_nan(&value) == 0);
	MG_ASSERT(mg_decimal_is_positive_infinity(&value) == 0);
	MG_ASSERT(mg_decimal_is_negative_infinity(&value) == 0);
	MG_ASSERT(mg_decimal_is_zero(&value) == 0);
	
	MG_ASSERT(mg_decimal_compare(&value, &value) == 0);
	
	MG_TEST_END();
}
