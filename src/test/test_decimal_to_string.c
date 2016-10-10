#include <string.h>
#include <magica/test.h>
#include <magica/decimal.h>

static void test(const char *text)
{
	mg_decimal value;
	char strbuf[1024];
	
	MG_DEBUG("target: %s\n", text);
	
	MG_OK(mg_decimal_parse_string(text, /*out*/&value));
	MG_OK(mg_decimal_to_string(&value, strbuf, 1024, NULL));
	
	MG_DEBUG("result: %s\n", strbuf);
	
	MG_ASSERT(strcmp(text, strbuf) == 0);
}

static void test_round(const char *text1, const char *text2)
{
	mg_decimal value;
	char strbuf[1024];
	
	MG_DEBUG("target: %s\n", text1);
	
	MG_OK(mg_decimal_parse_string(text1, /*out*/&value));
	MG_OK(mg_decimal_to_string(&value, strbuf, 1024, NULL));
	
	MG_DEBUG("result: %s\n", strbuf);
	
	MG_ASSERT(strcmp(text2, strbuf) == 0);
}

void test_mg_decimal_to_string()
{
	MG_TEST_BEGIN();
	
	test("0");
	test("1");
	test("2");
	test("3");
	test("4");
	test("5");
	test("6");
	test("7");
	test("8");
	test("9");
	test_round("-0", "0");
	test("-1");
	test("90");
	test("909");
	test("90998");
	test("909981236");
	test("909981236154561");
	test("1.000000000000000000000000000009");
	test("123456789123456798");
	test("123456789123456798.0");
	test("123456789123456798.0123456789123456");
	test("1234567891234567980000000000000000000000000000000000000000");
	test("0.00000000123456789123465798");
	test("0.000000000000000000000000000000000000000000000000000000000123456789123465798");
	test("12345678.91234567980123456789123456");
	test("1234567891234567980123456789.123456");
	test("9999999999999999999999900000000000000");
	test("999999999900000000000000");
	test("0.12345678912345679");
	test("0.12345678912345679123456");
	test("-0.000000000000000000000000000000000000000000000000000000000123456789123465798");
	test("-12345678.91234567980123456789123479");
	test("-1234567891234567980123456789.123479");
	test("-9999999999999999999999900000000000000");
	test("-999999999900000000000000");
	test("-0.12345678912345679");
	test("-0.12345678912345679123456");
	test("645105340.346846510101");
	test("64510.5340346846510101");
	test("6.45105340346846510101");
	test("645105340346846510.101");
	test("684150451654132021");
	test("684150132021");
	test("6841501");
	test("0.5");
	test("0.1");
	test("0.01");
	test("0.0001");
	
	test("9999999999999999999999999999999999");
	test("999999999999999999999999999999999.9");
	test("99999999999999999999999999.99999999");
	test("9999999999999999999.999999999999999");
	test("999999999999.9999999999999999999999");
	test("99999.99999999999999999999999999999");
	test("0.00009999999999999999999999999999999999");
	test("0.000000000000000009999999999999999999999999999999999");
	test("0.0000000000000000000000000000000009999999999999999999999999999999999");
	test("0.00000000000000000000000000000000000000000000000000000000000000009999999999999999999999999999999999");
	test("0.000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000009999999999999999999999999999999999");
	
	test("-9999999999999999999999999999999999");
	test("-999999999999999999999999999999999.9");
	test("-99999999999999999999999999.99999999");
	test("-9999999999999999999.999999999999999");
	test("-999999999999.9999999999999999999999");
	test("-99999.99999999999999999999999999999");
	test("-0.00009999999999999999999999999999999999");
	test("-0.000000000000000009999999999999999999999999999999999");
	test("-0.0000000000000000000000000000000009999999999999999999999999999999999");
	test("-0.00000000000000000000000000000000000000000000000000000000000000009999999999999999999999999999999999");
	test("-0.000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000009999999999999999999999999999999999");
	
	test("0.00000000000000000000000000000000000000000000000000000000000000001");
	test("-0.00000000000000000000000000000000000000000000000000000000000000001");
	
	test_round("+123456789123456", "123456789123456");
	test_round("9876543210987654321098765432109876543210", "9876543210987654321098765432109876000000");
	test_round("0.00000009876543210987654321098765432109876543210", "0.00000009876543210987654321098765432109876");
	test_round("0.0000000000000000000000000009876543210987654321098765432109876543210", "0.0000000000000000000000000009876543210987654321098765432109876");
	test_round("987654321098765.4321098765432109876543210", "987654321098765.4321098765432109876");
	test_round("1e-6177", "0");
	test_round("-1e-6177", "0");

	mg_decimal value;

	MG_ASSERT(mg_decimal_parse_string("9999999999999999999999999999999999e+6112", &value) != 0);
	MG_ASSERT(mg_decimal_parse_string("999999999999999999999999999999999e+6112", &value) == 0);
	MG_ASSERT(mg_decimal_parse_string("99999999999999999999999999999999e+6113", &value) == 0);

	//uint64_t low, high;
	//mg_decimal_parse_string("1", &value);
	//mg_decimal_pack(&value, /*out*/&low, /*out*/&high);
	//printf("0x%016llxULL, 0x%016llxULL\n", low, high);

	//mg_decimal_parse_string("-1", &value);
	//mg_decimal_pack(&value, /*out*/&low, /*out*/&high);
	//printf("0x%016llxULL, 0x%016llxULL\n", low, high);

	//mg_decimal_parse_string("1e-6176", &value);
	//mg_decimal_pack(&value, /*out*/&low, /*out*/&high);
	//printf("0x%016llxULL, 0x%016llxULL\n", low, high);

	//mg_decimal_parse_string("9999999999999999999999999999999999e+6111", &value);
	//mg_decimal_pack(&value, /*out*/&low, /*out*/&high);
	//printf("0x%016llxULL, 0x%016llxULL\n", low, high);

	MG_TEST_END();
}
