#include <magica/test.h>
#include <magica/decimal.h>

static void test(const char *text1, const char *ret)
{
	mg_decimal value1, value3, value4;

	MG_OK(mg_decimal_parse_string(text1, &value1));

	MG_OK(mg_decimal_abs(&value1, /*out*/&value3));

	MG_OK(mg_decimal_parse_string(ret, &value4));

	MG_ASSERT(mg_decimal_compare_1(&value3, &value4) == 0);
}

void test_decimal_abs()
{
	MG_TEST_BEGIN();

	test("-0", "0");
	test("0", "0");
	test("123456789", "123456789");
	test("-123456789", "123456789");
	test("123456789.0123456789", "123456789.0123456789");
	test("-123456789.0123456789", "123456789.0123456789");
	
	MG_TEST_END();
}
