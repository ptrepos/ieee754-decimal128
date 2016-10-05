#include <string.h>
#include <magica/test.h>
#include <magica/decimal.h>

static void value_test(unsigned long long value, const char *ret)
{
	char strbuf[1024];
	mg_decimal value1;
	unsigned long long value2;

	MG_OK(mg_decimal_value_of_uint64(value, /*out*/&value1));

	MG_OK(mg_decimal_to_uint64(&value1, /*int*/&value2));

	MG_OK(mg_decimal_to_string(&value1, /*out*/strbuf, 1024, NULL));
	
	MG_ASSERT(value == value2);
	MG_ASSERT(strcmp(strbuf, ret) == 0);
}

void test_decimal_value_of_uint64()
{
	MG_TEST_BEGIN();

	value_test(0, "0");
	value_test(1, "1");
	value_test(657984152, "657984152");
	value_test(UINT32_MAX, "4294967295");
	value_test(UINT64_MAX, "18446744073709551615");

	MG_TEST_END();
}
