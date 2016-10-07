#include <magica/test.h>
#include <magica/decimal.h>

void test_decimal_positive_infinity()
{
	MG_TEST_BEGIN();
	
	mg_decimal value;
	
	mg_decimal_positive_infinity(/*out*/&value);
	
	MG_ASSERT(mg_decimal_is_nan(&value) == 0);
	MG_ASSERT(mg_decimal_is_positive_infinity(&value) != 0);
	MG_ASSERT(mg_decimal_is_negative_infinity(&value) == 0);
	MG_ASSERT(mg_decimal_is_zero(&value) == 0);
	
	MG_TEST_END();
}
