#include <magica/test.h>
#include <magica/decimal.h>

void test_decimal_minus_one()
{
	MG_TEST_BEGIN();
	
	mg_decimal value;
	int value2;
	
	mg_decimal_minus_one(/*out*/&value);
	
	MG_ASSERT(mg_decimal_is_nan(&value) == 0);
	MG_ASSERT(mg_decimal_is_positive_infinity(&value) == 0);
	MG_ASSERT(mg_decimal_is_negative_infinity(&value) == 0);
	MG_ASSERT(mg_decimal_is_zero(&value) == 0);
	
	MG_ASSERT(mg_decimal_compare_1(&value, &value) == 0);
	
	MG_OK(mg_decimal_to_int(&value, /*out*/&value2));
	
	MG_ASSERT(value2 == -1);
	

	MG_TEST_END();
}
