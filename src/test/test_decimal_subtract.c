#include <magica/test.h>
#include <magica/decimal.h>

static void subtract_test(const char *text1, const char *text2, const char *ret)
{
	mg_decimal value1, value2, value3, value4;

	MG_OK(mg_decimal_parse_string(text1, &value1));
	MG_OK(mg_decimal_parse_string(text2, &value2));

	MG_OK(mg_decimal_subtract(&value1, &value2, /*out*/&value3));

	MG_OK(mg_decimal_parse_string(ret, &value4));

	MG_ASSERT(mg_decimal_compare(&value3, &value4) == 0);
}

void test_decimal_subtract()
{
	MG_TEST_BEGIN();
	
	subtract_test("1", "1", "0");
	subtract_test("1", "-1", "2");
	subtract_test("1", "0", "1");
	subtract_test("0", "1", "-1");
	subtract_test("100000000000000000000", "10000000000000000", "99990000000000000000");
	subtract_test("100000000000000000000", "-10000000000000000", "100010000000000000000");
	subtract_test("-1000000.000001", "99.99", "-1000099.990001");
	subtract_test("1000000000.00000000000000001", "999999999.9999999999999999", "0.00000000000000011");
	subtract_test("1000000000.00000000000000001", "-999999999.9999999999999999", "1999999999.99999999999999991");
	subtract_test("9999999999999999999999999999999998", "9999999999999999999999999999999999", "-1");
	subtract_test("123456789123456789123456789", "-0.123456789123456789123456789", "123456789123456789123456789.123456789");
	subtract_test("99999999999999999999999", "9999999999999999999999999", "-9900000000000000000000000");

	MG_TEST_END();
}
