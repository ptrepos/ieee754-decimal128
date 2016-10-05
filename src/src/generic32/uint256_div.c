/**
 * Copyright (C) Takuo Hada 2015-2016
 * @author t.hada 2016/07/09
 *
 * 256 bit integer for decimal.
 * this source includes decimal implementation.
 */
#include <assert.h>
#include <magica/int/mgcompat.h>
#include <magica/int/uint64.h>
#include <magica/int/uint256_def.h>
#include <magica/int/uint256_impl.h>

// 長除法による実装
int mg_uint256_div_long_division_impl(
	mg_uint256 *op1, 
	int op1_digits, 
	const mg_uint256 *op2, 
	int op2_digits, 
	mg_uint256 *quotient);

MG_PRIVATE int mg_uint256_div_maclaurin_impl(
	mg_uint256 *op1,
	const mg_uint256 *op2,
	int op2_bits,
	/*out*/mg_uint256 *quotient);

int mg_uint256_div(const mg_uint256 *op1, const mg_uint256 *op2, mg_uint256 *quotient, mg_uint256 *reminder)
{
	int err = 0;

	mg_uint256 _op1 = *op1;

	int op1_digits = MG_UINT256_SIZE;
	while(op1_digits > 0 && _op1.word[op1_digits-1] == 0)
		op1_digits--;

	if(op1_digits <= 0) {
		mg_uint256_set_zero(quotient);
		mg_uint256_set_zero(reminder);
		goto _EXIT;
	}

	int op2_digits = MG_UINT256_SIZE;
	while(op2_digits > 0 && op2->word[op2_digits-1] == 0)
		op2_digits--;

	if(op2_digits <= 1) {
		if(op2_digits <= 0) {
			err = 1;
			goto _ERROR;
		}
		if(op1_digits <= 1) {
			uint64_t q = _op1.word[0] / op2->word[0];
			uint64_t r = _op1.word[0] % op2->word[0];
			mg_uint256_set(/*out*/quotient, q);
			mg_uint256_set(/*out*/reminder, r);
			goto _EXIT;
		}
	}

	err = mg_uint256_div_long_division_impl(
			/*inout*/&_op1, op1_digits,
			op2, op2_digits,
			/*out*/quotient);
	if (err != 0)
		goto _ERROR;

	*reminder = _op1;
	
_EXIT:
	return 0;
_ERROR:
	return err;
}

MG_PRIVATE int mg_uint256_div_maclaurin(
	const mg_uint256 *op1,
	const mg_uint256 *op2,
	/*out*/mg_uint256 *quotient,
	/*out*/mg_uint256 *reminder)
{
	int err = 0;

	mg_uint256 _op1 = *op1;

	int op2_bits = mg_uint256_get_bit_size(op2);

	if (op2_bits <= 0) {
		err = 1;
		goto _ERROR;
	}
	if (mg_uint256_compare(&_op1, op2) < 0) {
		mg_uint256_set_zero(quotient);
		*reminder = _op1;
		goto _EXIT;
	}

	err = mg_uint256_div_maclaurin_impl(
		/*inout*/&_op1,
		op2, op2_bits,
		/*out*/quotient);
	if (err != 0)
		goto _ERROR;

	*reminder = _op1;

_EXIT:
	return 0;
_ERROR:
	return err;
}

typedef long double max_float_t;

#define DOUBLE_RSHIFT_32		(1.0/4294967296.0)
#define DOUBLE_LSHIFT_32		(4294967296.0)
#define DOUBLE_CORRECT			(0.9999)

static inline void set_double(mg_uint256 *op1, max_float_t value, int n)
{
	mg_uint256_set_zero(op1);
	if (n == 0) {
		op1->word[n] = (uint32_t) value;
	} else if(n == 1) {
		op1->word[n] = (uint32_t) value;
		value -= op1->word[n];
		value *= DOUBLE_LSHIFT_32;
		op1->word[n - 1] = (uint32_t)value;
	} else {
		op1->word[n] = (uint32_t) value;
		value -= op1->word[n];
		value *= DOUBLE_LSHIFT_32;
		op1->word[n - 1] = (uint32_t)value;
		value -= op1->word[n - 1];
		value *= DOUBLE_LSHIFT_32;
		op1->word[n - 2] = (uint32_t)value;
	}
}

MG_PRIVATE int mg_uint256_div_long_division_impl(
				mg_uint256 *op1, 
				int op1_digits, 
				const mg_uint256 *op2, 
				int op2_digits, 
				mg_uint256 *quotient)
{
	//mg_decimal_error err = 0;
	mg_uint256 buf1, buf2, buf3, tmp;
	mg_uint256 *q = &buf1, *qv = &buf2, *qv_hi = &buf3;

	max_float_t op2_v = (max_float_t)op2->word[op2_digits -1];
	if(op2_digits >= 2)
		op2_v += (max_float_t)op2->word[op2_digits-2] * DOUBLE_RSHIFT_32;

	mg_uint256_set_zero(/*out*/quotient);

	while (op1_digits >= op2_digits && mg_uint256_compare(op1, op2) >= 0) {
		if(op2_digits <= 1 && op1_digits <= 1) {
			uint64_t iq = op1->word[0] / op2->word[0];
			uint64_t ir = op1->word[0] % op2->word[0];
			mg_uint256_set(q, iq);
			mg_uint256_set(op1, ir);

			mg_uint256_add(quotient, q, /*out*/&tmp);
			*quotient = tmp;
			goto _EXIT;
		}

		max_float_t op1_v = (max_float_t)op1->word[op1_digits-1];
		if(op1_digits >= 2)
			op1_v += (max_float_t)op1->word[op1_digits-2] * DOUBLE_RSHIFT_32;

		int q_n = op1_digits - op2_digits;
		max_float_t q_tmp = op1_v / op2_v;
		if((q_tmp < 1.0 && q_n == 0)) {
			q_tmp = 1.0;
		}
		// オーバーフロー防止
		if(q_tmp >= DOUBLE_LSHIFT_32) {
			q_tmp *= DOUBLE_CORRECT;
		}

		set_double(q, q_tmp, q_n);

		int overflow = mg_uint256_mul_digits(
			op2, op2_digits, q, q_n + 1, /*out*/qv);

		while(overflow || mg_uint256_sub(op1, qv, &tmp) != 0) {
			q_tmp *= DOUBLE_CORRECT;
			set_double(q, q_tmp, q_n);

			overflow = mg_uint256_mul_digits(
				op2, op2_digits, q, q_n + 1, /*out*/qv);
		}
		*op1 = tmp;

		mg_uint256_add(quotient, q, /*out*/&tmp);
		*quotient = tmp;

		while(op1_digits > 0 && op1->word[op1_digits-1] == 0)
			op1_digits--;
	}
_EXIT:
	return 0;
//_ERROR:
//	return err;
}

MG_PRIVATE int mg_uint256_div_maclaurin_impl(
	mg_uint256 *op1,
	const mg_uint256 *op2,
	int op2_bits,
	/*out*/mg_uint256 *quotient)
{
	//mg_decimal_error err;
	mg_uint256 tmp, tmp2, tmp3, dummy;
	int overflow;
	int bits = op2_bits;

	// x = 1 - (op2 / BASE)
	mg_uint256 b;
	mg_uint256_left_shift(op2, 256 - op2_bits, /*out*/&b);
	mg_uint256 x;
	mg_uint256_neg(&b, /*out*/&x);

	// (1 + x)
	mg_uint256 xn = x;
	mg_uint256 series = xn;	// 1を省略

	// (1 + x^2)(1 + x^4)(1 + x^8)...(1 + x^2n)
	while (mg_uint256_is_zero(&xn) == 0) {
		// x^2n計算
		mg_uint256_mul_1(&xn, &xn, /*out*/&dummy, /*out*/&tmp);
		xn = tmp;

		// series * (1 + x^2n) = (series + series * x^2n)
		mg_uint256_mul_1(&series, &xn, /*out*/&dummy, /*out*/&tmp);
		overflow = mg_uint256_add(&series, &tmp, /*out*/&tmp2);
		assert(!overflow);
		overflow = mg_uint256_add(&tmp2, &xn, /*out*/&series);		// 省略した1の分、1 * xnを加算
		assert(!overflow);
	}

	mg_uint256 q;
	mg_uint256_mul_1(op1, &series, /*out*/&dummy, /*out*/&tmp);
	mg_uint256_right_shift(&tmp, op2_bits, &tmp2);
	mg_uint256_right_shift(op1, op2_bits, &tmp3);
	overflow = mg_uint256_add(&tmp2, &tmp3, /*out*/&q);		// 省略した1の分を加算
	assert(!overflow);

	// r = op1 - q * op2;
	mg_uint256 r;
	mg_uint256_mul(&q, op2, /*out*/&tmp);
	overflow = mg_uint256_sub(op1, &tmp, /*out*/&r);
	assert(!overflow);

	// qの精度補正
	mg_uint256_set(&tmp2, 1);
	while (mg_uint256_sub(&r, op2, &tmp) == 0) {
		r = tmp;
		mg_uint256_add(&q, &tmp2, /*out*/&tmp);
		q = tmp;
	}

	*quotient = q;
	*op1 = r;

	return 0;
	//_ERROR:
	//	return err;
}
