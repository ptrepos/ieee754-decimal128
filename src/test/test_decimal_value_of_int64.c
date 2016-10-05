#include <string.h>
#include <magica/test.h>
#include <magica/decimal.h>

static void value_test(int64_t value, const char *ret)
{
	char strbuf[1024];
	mg_decimal value1;
	int64_t value2;

	MG_OK(mg_decimal_value_of_int64(value, /*out*/&value1));

	MG_OK(mg_decimal_to_int64(&value1, /*int*/&value2));

	MG_OK(mg_decimal_to_string(&value1, /*out*/strbuf, 1024, NULL));
	
	MG_ASSERT(value == value2);
	MG_ASSERT(strcmp(strbuf, ret) == 0);
}

void test_decimal_value_of_int64()
{
	MG_TEST_BEGIN();

	value_test(0, "0");
	value_test(1, "1");
	value_test(-1, "-1");
	value_test(1, "1");
	value_test(657984152, "657984152");
	value_test(-657984152, "-657984152");
	value_test(INT32_MAX, "2147483647");
	value_test(INT32_MIN, "-2147483648");
	value_test(INT64_MAX, "9223372036854775807");
	value_test(INT64_MIN, "-9223372036854775808");

	MG_TEST_END();
}
