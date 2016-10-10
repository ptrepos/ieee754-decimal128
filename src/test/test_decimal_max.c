#include <magica/test.h>
#include <magica/decimal.h>

static void test(const char *text1, const char *text2, const char *ret)
{
	mg_decimal value1, value2, value3, value4;

	MG_OK(mg_decimal_parse_string(text1, &value1));
	MG_OK(mg_decimal_parse_string(text2, &value2));

	MG_OK(mg_decimal_max(&value1, &value2, /*out*/&value3));

	MG_OK(mg_decimal_parse_string(ret, &value4));

	MG_ASSERT(mg_decimal_compare_1(&value3, &value4) == 0);
}

void test_decimal_max()
{
	MG_TEST_BEGIN();

	test("-1", "1", "1");
	test("-0", "0", "0");
	test("123456789", "123456790", "123456790");
	test("123456789000000000000000000000000000000000000000", "12345679000000000000000000000000000000000000000", "123456789000000000000000000000000000000000000000");
	test("0.0000000000000000000000000000000000000000000001", "0.000000000000000000000000000000000000000000001", "0.000000000000000000000000000000000000000000001");
	test("0.0000000000000000000000000000000000000000000001", "0.00000000000000000000000000000000000000000000001", "0.0000000000000000000000000000000000000000000001");
	
	MG_TEST_END();
}
