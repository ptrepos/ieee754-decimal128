#include <magica/test.h>
#include <magica/decimal.h>

void test_mg_decimal_to_string();
void test_decimal_value_of_int();
void test_decimal_value_of_uint();
void test_decimal_value_of_int64();
void test_decimal_value_of_uint64();
void test_decimal_value_of_float();
void test_decimal_value_of_double();
void test_mg_decimal_compare();
void test_decimal_add();
void test_decimal_subtract();
void test_mg_decimal_multiply();
void test_decimal_divide();

void test_decimal()
{
	test_mg_decimal_to_string();
	test_decimal_value_of_int();
	test_decimal_value_of_uint();
	test_decimal_value_of_int64();
	test_decimal_value_of_uint64();
	test_decimal_value_of_float();
	test_decimal_value_of_double();
	test_mg_decimal_compare();
	test_decimal_add();
	test_decimal_subtract();
	test_mg_decimal_multiply();
	test_decimal_divide();
}