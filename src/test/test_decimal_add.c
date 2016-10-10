#include <magica/test.h>
#include <magica/decimal.h>

static void add_test(const char *text1, const char *text2, const char *ret)
{
	mg_decimal value1, value2, value3, value4;

	MG_OK(mg_decimal_parse_string(text1, &value1));
	MG_OK(mg_decimal_parse_string(text2, &value2));

	MG_OK(mg_decimal_add(&value1, &value2, /*out*/&value3));

	MG_OK(mg_decimal_parse_string(ret, &value4));
	
	MG_ASSERT(mg_decimal_compare_1(&value3, &value4) == 0);
}

static void add_infinity_test(const char *text1)
{
	mg_decimal value1, value2, value3;

	MG_OK(mg_decimal_parse_string(text1, &value1));
	mg_decimal_positive_infinity(&value2);

	MG_OK(mg_decimal_add(&value1, &value2, /*out*/&value3) == 0);
	MG_ASSERT(mg_decimal_is_positive_infinity(&value3));

	MG_OK(mg_decimal_add(&value2, &value1, /*out*/&value3) == 0);
	MG_ASSERT(mg_decimal_is_positive_infinity(&value3));
	
	mg_decimal_negative_infinity(&value2);

	MG_OK(mg_decimal_add(&value1, &value2, /*out*/&value3) == 0);
	MG_ASSERT(mg_decimal_is_negative_infinity(&value3));

	MG_OK(mg_decimal_add(&value2, &value1, /*out*/&value3) == 0);
	MG_ASSERT(mg_decimal_is_negative_infinity(&value3));
}

static void add_positive_overflow_test(const char *text1, const char *text2)
{
	mg_decimal value1, value2, value3;

	MG_OK(mg_decimal_parse_string(text1, &value1));
	MG_OK(mg_decimal_parse_string(text1, &value2));

	MG_OK(mg_decimal_add(&value1, &value2, /*out*/&value3) == 0);
	MG_ASSERT(mg_decimal_is_positive_infinity(&value3));
}

static void add_negative_overflow_test(const char *text1, const char *text2)
{
	mg_decimal value1, value2, value3;

	MG_OK(mg_decimal_parse_string(text1, &value1));
	MG_OK(mg_decimal_parse_string(text1, &value2));

	MG_OK(mg_decimal_add(&value1, &value2, /*out*/&value3) == 0);
	MG_ASSERT(mg_decimal_is_negative_infinity(&value3));
}

void test_decimal_add()
{
	MG_TEST_BEGIN();

	add_test("1", "1", "2");
	add_test("1", "-1", "0");
	add_test("1", "0", "1");
	add_test("0", "1", "1");
	add_test("100000000000000000000", "10000000000000000", "100010000000000000000");
	add_test("100000000000000000000", "-10000000000000000", "99990000000000000000");
	add_test("-100000000000000000000", "10000000000000000", "-99990000000000000000");
	add_test("-100000000000000000000", "-10000000000000000", "-100010000000000000000");
	add_test("-1000000.000001", "-99.99", "-1000099.990001");
	add_test("-1000000.000001", "-99.000001", "-1000099.000002");
	add_test("1000000000.00000000000000001", "999999999.9999999999999999", "1999999999.99999999999999991");
	add_test("1000000000.00000000000000001", "-999999999.9999999999999999", "0.00000000000000011");
	add_test("-1000000000.00000000000000001", "999999999.9999999999999999", "-0.00000000000000011");
	add_test("-1000000000.00000000000000001", "-999999999.9999999999999999", "-1999999999.99999999999999991");
	add_test("9999999999999999999999999999999998", "1", "9999999999999999999999999999999999");
	add_test("3295", "8", "3303");
	add_test("9996666", "3334", "10000000");
	add_test("99999999999999999999", "1", "100000000000000000000");

	add_test("9999999999999999999900000000000000000000", "100000000000000000000", "10000000000000000000000000000000000000000");

	add_test("0.00000000000000000000000000000099999999999999999999", "0.00000000000000000000000000000000000000000000000001", "0.000000000000000000000000000001");

	add_test("1", "0.000000000000001", "1.000000000000001");
	add_test("1", "0.0000000000000000000000001", "1.0000000000000000000000001");
	add_test("1", "0.000000000000000000000000000000001", "1.000000000000000000000000000000001");
	add_test("1", "0.0000000000000000000000000000000001", "1");
	add_test("1", "0.0000000000000000000000000000000000000000000000000000000000000000001", "1");
	add_test("0.000000000000001", "1", "1.000000000000001");
	add_test("0.0000000000000000000000001", "1", "1.0000000000000000000000001");
	add_test("0.000000000000000000000000000000001", "1", "1.000000000000000000000000000000001");
	add_test("0.0000000000000000000000000000000001","1", "1");
	add_test("0.0000000000000000000000000000000000000000000000000000000000000000001", "1", "1");

	add_test("9999999999999999999999999999999999", "1", "10000000000000000000000000000000000");
	add_test("99999999999999999999999999999999990", "10", "100000000000000000000000000000000000");
	add_test("9999999999999999999999999999999999", "9999999999999999999999999999999999", "19999999999999999999999999999999990");

	add_infinity_test("0.0000000000000000000000000000000000000000000000000000000000000000001");
	add_infinity_test("0.0000000000000000000000001");
	add_infinity_test("1");
	add_infinity_test("100000000000000000");
	add_infinity_test("1000000000000000000000000000000000000000");
	add_infinity_test("100000000000000000000000000000000000000000000000000000000000000000000000000");

	add_positive_overflow_test("9999999999999999999999999999999999e+6111", "9999999999999999999999999999999999e+6111");
	add_positive_overflow_test("9999999999999999999999999999999999e+6111", "9999999999999999999999999999999999e+6080");

	add_negative_overflow_test("-9999999999999999999999999999999999e+6111", "-9999999999999999999999999999999999e+6111");
	add_negative_overflow_test("-9999999999999999999999999999999999e+6111", "-9999999999999999999999999999999999e+6080");

	MG_TEST_END();
}
