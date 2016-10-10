#include <magica/test.h>
#include <magica/decimal.h>

void test_mg_decimal_to_string();
void test_decimal_value_of_int();
void test_decimal_value_of_uint();
void test_decimal_value_of_int64();
void test_decimal_value_of_uint64();
void test_decimal_value_of_float();
void test_decimal_value_of_double();
void test_mg_decimal_compare_1();
void test_decimal_add();
void test_decimal_subtract();
void test_mg_decimal_multiply();
void test_decimal_divide();
void test_mg_decimal_round_up();
void test_mg_decimal_round_off();
void test_mg_decimal_round_down();
void test_mg_decimal_ceiling();
void test_mg_decimal_floor();
void test_decimal_abs();
void test_decimal_max();
void test_decimal_min();
void test_decimal_zero();
void test_decimal_nan();
void test_decimal_positive_infinity();
void test_decimal_negative_infinity();
void test_decimal_one();
void test_decimal_minus_one();
void test_decimal_min_value();
void test_decimal_max_value();

void test_decimal()
{
	test_mg_decimal_to_string();
	test_decimal_value_of_int();
	test_decimal_value_of_uint();
	test_decimal_value_of_int64();
	test_decimal_value_of_uint64();
	test_decimal_value_of_float();
	test_decimal_value_of_double();
	test_mg_decimal_compare_1();
	test_decimal_add();
	test_decimal_subtract();
	test_mg_decimal_multiply();
	test_decimal_divide();
	test_mg_decimal_round_up();
	test_mg_decimal_round_off();
	test_mg_decimal_round_down();
	test_mg_decimal_ceiling();
	test_mg_decimal_floor();
	test_decimal_abs();
	test_decimal_max();
	test_decimal_min();
	test_decimal_zero();
	test_decimal_nan();
	test_decimal_positive_infinity();
	test_decimal_negative_infinity();
	test_decimal_one();
	test_decimal_minus_one();
	test_decimal_min_value();
	test_decimal_max_value();
}
