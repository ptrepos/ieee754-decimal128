#include <string.h>
#include <math.h>
#include <magica/test.h>
#include <magica/decimal.h>

static void value_test(double value, double diff)
{
	char strbuf[1024];
	mg_decimal value1;
	double value2;

	MG_OK(mg_decimal_value_of_double(value, /*out*/&value1));

	MG_OK(mg_decimal_to_double(&value1, /*int*/&value2));

	MG_OK(mg_decimal_to_string(&value1, /*out*/strbuf, 1024, NULL));

	MG_DEBUG("data: %s\n", strbuf);
	MG_DEBUG("test: %.17f\n", value);

	MG_ASSERT(fabs(value - value2) <= diff);
}

void test_decimal_value_of_double()
{
	MG_TEST_BEGIN();

	value_test(0, 0);
	value_test(1, 0);
	value_test(-1, 0);
	value_test(1, 0);
	value_test(657984152, 0);
	value_test(-657984152, 0);
	value_test(INT32_MAX, 0);
	value_test(INT32_MIN, 0);
	value_test(100000.123456798, 0.000001);
	value_test(1000000000000000000000.0, 100000.0);
	value_test(0.0000000000000000000001, 0.000000000000000000000000000000000001);
	value_test(6579841526579841526579841526579841.0, 10000000000000000.0);
	value_test(6579841526579841526579841526579841123.0, 10000000000000000000.0);
	value_test(6579841526579841526579841526579841123456123.0, 10000000000000000000000000.0);

	MG_TEST_END();
}
