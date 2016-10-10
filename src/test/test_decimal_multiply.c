#include <string.h>
#include <magica/test.h>
#include <magica/decimal.h>

static void multiply_test(const char *text1, const char *text2, const char *ret)
{
	char strbuf[1000];
	mg_decimal value1, value2, value3, value4;

	MG_OK(mg_decimal_parse_string(text1, &value1));
	MG_OK(mg_decimal_parse_string(text2, &value2));

	MG_OK(mg_decimal_multiply(&value1, &value2, /*out*/&value3));

	MG_OK(mg_decimal_to_string(&value3, strbuf, 1000, NULL));

	MG_OK(mg_decimal_parse_string(ret, &value4));

	MG_ASSERT(mg_decimal_compare_1(&value3, &value4) == 0);
}

void test_mg_decimal_multiply()
{
	MG_TEST_BEGIN();
	
	multiply_test("1000000" , "1000000", "1000000000000");
	multiply_test("1000000000000", "1000000000000", "1000000000000000000000000");
	multiply_test("-1000000", "1000000", "-1000000000000");
	multiply_test("1000000000000", "-1000000000000", "-1000000000000000000000000");
	multiply_test("1000000000000", "1000000", "1000000000000000000");
	multiply_test("1000000", "0.000001", "1");
	multiply_test("1000000000000", "0.000000000001", "1");
	multiply_test("1000000", "10.5", "10500000");
	multiply_test("1000000000000000000000000000000000000", "0.000000000000000000000000000000000001", "1");
	multiply_test("1000000000000000000000000000000000000000000000000000000000000000000000000000000", "0.000000000000000000000000000000000000000000000000000000000000000000000000000001", "1");

	MG_TEST_END();
}
