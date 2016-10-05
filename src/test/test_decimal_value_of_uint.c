#include <string.h>
#include <magica/test.h>
#include <magica/decimal.h>

static void value_test(unsigned int value, const char *ret)
{
	char strbuf[1024];
	mg_decimal value1;
	unsigned int value2;

	MG_OK(mg_decimal_value_of_uint(value, /*out*/&value1));

	MG_OK(mg_decimal_to_uint(&value1, /*int*/&value2));

	MG_OK(mg_decimal_to_string(&value1, /*out*/strbuf, 1024, NULL));
	
	MG_ASSERT(value == value2);
	MG_ASSERT(strcmp(strbuf, ret) == 0);
}

void test_decimal_value_of_uint()
{
	MG_TEST_BEGIN();

	value_test(0, "0");
	value_test(1, "1");
	value_test(657984152, "657984152");
	value_test(UINT32_MAX, "4294967295");

	MG_TEST_END();
}
