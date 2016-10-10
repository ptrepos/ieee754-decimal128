#include <string.h>
#include <magica/test.h>
#include <magica/decimal.h>

static void compare_test(const char *text1, const char *text2, int ret)
{
	mg_decimal value1, value2;

	MG_OK(mg_decimal_parse_string(text1, &value1));
	MG_OK(mg_decimal_parse_string(text2, &value2));

	if(ret < 0) {
		MG_ASSERT(mg_decimal_compare_1(&value1, &value2) < 0);
	} else if(ret == 0) {
		MG_ASSERT(mg_decimal_compare_1(&value1, &value2) == 0);
	} else {
		MG_ASSERT(mg_decimal_compare_1(&value1, &value2) > 0);
	}
}

static void compare_positive_infinity_test(const char *text1)
{
	int ret;
	mg_decimal value1, value2;

	MG_OK(mg_decimal_parse_string(text1, &value1));
	mg_decimal_positive_infinity(&value2);

	MG_OK(mg_decimal_compare(&value1, &value2, /*out*/&ret));
	MG_ASSERT(ret < 0);
	MG_OK(mg_decimal_compare(&value2, &value1, /*out*/&ret));
	MG_ASSERT(ret > 0);
}

static void compare_negative_infinity_test(const char *text1)
{
	int ret;
	mg_decimal value1, value2;

	MG_OK(mg_decimal_parse_string(text1, &value1));
	mg_decimal_negative_infinity(&value2);

	MG_OK(mg_decimal_compare(&value1, &value2, /*out*/&ret));
	MG_ASSERT(ret > 0);
	MG_OK(mg_decimal_compare(&value2, &value1, /*out*/&ret));
	MG_ASSERT(ret < 0);
}

static void compare_infinity_test()
{
	int ret;
	mg_decimal value1, value2;

	mg_decimal_positive_infinity(&value1);
	mg_decimal_positive_infinity(&value2);

	MG_OK(mg_decimal_compare(&value1, &value2, /*out*/&ret));
	MG_ASSERT(ret == 0);

	mg_decimal_negative_infinity(&value1);
	mg_decimal_negative_infinity(&value2);

	MG_OK(mg_decimal_compare(&value1, &value2, /*out*/&ret));
	MG_ASSERT(ret == 0);

	mg_decimal_positive_infinity(&value1);
	mg_decimal_negative_infinity(&value2);

	MG_OK(mg_decimal_compare(&value1, &value2, /*out*/&ret));
	MG_ASSERT(ret > 0);

	mg_decimal_negative_infinity(&value1);
	mg_decimal_positive_infinity(&value2);

	MG_OK(mg_decimal_compare(&value1, &value2, /*out*/&ret));
	MG_ASSERT(ret < 0);
}

static void compare_nan_test(const char *text1)
{
	int ret;
	mg_decimal value1, value2;

	MG_OK(mg_decimal_parse_string(text1, &value1));
	mg_decimal_nan(&value2);

	MG_ASSERT(mg_decimal_compare(&value1, &value2, /*out*/&ret) != 0);
	MG_ASSERT(mg_decimal_compare(&value2, &value1, /*out*/&ret) != 0);
}

void test_mg_decimal_compare_1()
{
	MG_TEST_BEGIN();
	
	compare_test("1000", "1000", 0);
	compare_test("1000000000000000000000000000000", "1000000000000000000000000000000", 0);
	compare_test("1000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000", "1000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000", 0);
	compare_test("1000", "1000.0000000", 0);
	compare_test("1000", "1000.0000000000000", 0);
	compare_test("9999", "9999", 0);
	compare_test("9999", "9999.000000000000000000000", 0);
	compare_test("123456789123456789", "123456789123456789", 0);
	compare_test("123456789123456789.112635", "123456789123456789.112635", 0);
	compare_test("1.0", "1.0000000000000000000000000000000000000000000000000000000000000000000", 0);

	compare_test("9999", "9999.0000000000001", -1);
	compare_test("9999.0000000000001", "9999", 1);
	compare_test("1.0", "9999", -1);

	compare_test("0.0000000000000000000000000000000000000000000000000000000000000000001", "0.0000000000000000000000000000000000000000000000000000000000000000001", 0);
	compare_test("0.000000000000000000000000000000000000000000000000000000000000000001", "0.0000000000000000000000000000000000000000000000000000000000000000001", 1);
	compare_test("0.0000000000000000000000000000000000000000000000000000000000000000001", "0.000000000000000000000000000000000000000000000000000000000000000001", -1);

	compare_nan_test("0");
	compare_nan_test("123456");
	compare_nan_test("123456999999999");
	compare_nan_test("0.000000001");
	compare_nan_test("-123456");
	compare_nan_test("-123456999999999");
	compare_nan_test("-0.000000001");

	compare_positive_infinity_test("0");
	compare_positive_infinity_test("123456");
	compare_positive_infinity_test("123456999999999");
	compare_positive_infinity_test("0.000000001");
	compare_positive_infinity_test("-123456");
	compare_positive_infinity_test("-123456999999999");
	compare_positive_infinity_test("-0.000000001");
	compare_positive_infinity_test("9999999999999999999999999999999999999999999999999999990000000000000000000000000000000000");
	compare_positive_infinity_test("-9999999999999999999999999999999999999999999999999999990000000000000000000000000000000000");

	compare_negative_infinity_test("0");
	compare_negative_infinity_test("123456");
	compare_negative_infinity_test("123456999999999");
	compare_negative_infinity_test("0.000000001");
	compare_negative_infinity_test("-123456");
	compare_negative_infinity_test("-123456999999999");
	compare_negative_infinity_test("-0.000000001");
	compare_negative_infinity_test("9999999999999999999999999999999999999999999999999999990000000000000000000000000000000000");
	compare_negative_infinity_test("-9999999999999999999999999999999999999999999999999999990000000000000000000000000000000000");

	compare_infinity_test();

	MG_TEST_END();
}
