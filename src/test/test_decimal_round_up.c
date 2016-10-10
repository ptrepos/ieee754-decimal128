#include <string.h>
#include <magica/test.h>
#include <magica/decimal.h>

static void test(const char *text, int precision, const char *ret)
{
	mg_decimal value;
	char strbuf[1024];
	
	MG_OK(mg_decimal_parse_string(text, /*out*/&value));
	
	MG_OK(mg_decimal_round_up(&value, precision, /*out*/&value));
	
	MG_OK(mg_decimal_to_string(&value, strbuf, 1024, NULL));
	
	MG_ASSERT(strcmp(text, strbuf) == 0);
}

void test_mg_decimal_round_up()
{
	MG_TEST_BEGIN();

	test("123456789", 0, "123456789");
	test("1234567890000000000000000000000000000000000000000000000000", 0, "1234567890000000000000000000000000000000000000000000000000");
	test("10.0015", 3, "10.002");
	test("10.0014", 3, "10.001");
	test("10.000000000000000000000015", 23, "10.00000000000000000000002");
	test("10.000000000000000000000014", 23, "10.00000000000000000000002");
	test("10.00000000000000000000000000000015", 31, "10.0000000000000000000000000000002");
	test("10.00000000000000000000000000000014", 31, "10.0000000000000000000000000000002");
	test("0.0000000000000000000000000000000000000015", 39, "0.000000000000000000000000000000000000002");
	test("0.0000000000000000000000000000000000000014", 39, "0.000000000000000000000000000000000000002");
	test("0.0000000000000000000000000000000000000010", 39, "0.000000000000000000000000000000000000001");
	
	MG_TEST_END();
}