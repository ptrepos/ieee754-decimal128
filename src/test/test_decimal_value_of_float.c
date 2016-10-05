#include <string.h>
#include <math.h>
#include <magica/test.h>
#include <magica/decimal.h>

static void value_test(float value, float diff)
{
	char strbuf[1024];
	mg_decimal value1;
	float value2;

	MG_OK(mg_decimal_value_of_float(value, /*out*/&value1));

	MG_OK(mg_decimal_to_float(&value1, /*int*/&value2));

	MG_OK(mg_decimal_to_string(&value1, /*out*/strbuf, 1024, NULL));

	MG_DEBUG("data: %s\n", strbuf);
	MG_DEBUG("test: %.17f\n", value);

	MG_ASSERT(fabs(value - value2) <= diff);
}

void test_decimal_value_of_float()
{
	MG_TEST_BEGIN();

	value_test(0, 0);
	value_test(1, 0);
	value_test(-1, 0);
	value_test(1, 0);
	value_test(657984152.0f, 100);
	value_test(-657984152.0f, 100);
	value_test(100000.123456798f, 0.000001f);
	value_test(1000000000000000000000.0f, 100000.0f);
	value_test(0.0000000000000000000001f, 0.0000000000000000000000001f);

	MG_TEST_END();
}
