/**
 * Copyright (C) Takuo Hada 2015-2016
 * @author t.hada 2016/08/27
 *
 * 128 bit integer for decimal.
 */
#pragma once

#include <magica/int/uint32.h>
#include <magica/int/uint64.h>
#include <magica/int/uint128_def.h>

#include "intop.h"

#ifdef __cplusplus
extern "C" {
#endif

int mg_uint128_mul_digits(const mg_uint128 *op1, int op1_digits, const mg_uint128 *op2, int op2_digits, /*out*/mg_uint128 *ret);
void mg_uint128_mul_digits_1(const mg_uint128 *op1, int op1_words, const mg_uint128 *op2, int op2_words, /*out*/mg_uint128 *low, /*out*/mg_uint128 *high);

int mg_uint128_div(
	const mg_uint128 *op1, 
	const mg_uint128 *op2, 
	/*out*/mg_uint128 *quotient, 
	/*out*/mg_uint128 *reminder);
int mg_uint128_div_long_division(
	const mg_uint128 *op1, 
	const mg_uint128 *op2, 
	/*out*/mg_uint128 *quotient, 
	/*out*/mg_uint128 *reminder);
int mg_uint128_div_maclaurin(
	const mg_uint128 *op1, 
	const mg_uint128 *op2, 
	/*out*/mg_uint128 *quotient, 
	/*out*/mg_uint128 *reminder);

void mg_uint128_test_to_string(const mg_uint128 *value, char *buf);
void mg_uint128_test_convert(const char *buf, mg_uint128 *value);
void mg_uint128_test_to_hex_string(const mg_uint128 *value, char *buf);
void mg_uint128_test_hex_convert(const char *buf, mg_uint128 *value);

static inline void mg_uint128_set_zero(mg_uint128 *dest)
{
	dest->word[0] = 0;
	dest->word[1] = 0;
	dest->word[2] = 0;
	dest->word[3] = 0;
}

static inline void mg_uint128_set(mg_uint128 *dest, uint64_t value)
{
	dest->word[0] = (uint32_t)value;
	dest->word[1] = (uint32_t)(value >> 32);
	dest->word[2] = 0;
	dest->word[3] = 0;
}

static inline uint64_t mg_uint128_get_uint64(const mg_uint128 *value)
{
	return (uint64_t)value->word[0] | ((uint64_t)value->word[1] << 32);
}

static inline void mg_uint128_swap(mg_uint128 **a, mg_uint128 **b)
{
	mg_uint128 *tmp = *a;
	*a = *b;
	*b = tmp;
}

static inline int mg_uint128_is_zero(const mg_uint128 *value)
{
	return (value->word[3] | value->word[2] | value->word[1] | value->word[0]) == 0;
}

static inline int mg_uint128_compare(const mg_uint128 *value1, const mg_uint128 *value2)
{
	if (value1->word[3] != value2->word[3])
		return value1->word[3] < value2->word[3] ? -1 : 1;
	if (value1->word[2] != value2->word[2])
		return value1->word[2] < value2->word[2] ? -1 : 1;
	if (value1->word[1] != value2->word[1])
		return value1->word[1] < value2->word[1] ? -1 : 1;
	if (value1->word[0] != value2->word[0])
		return value1->word[0] < value2->word[0] ? -1 : 1;
	return 0;
}

static inline int mg_uint128_add(const mg_uint128 *op1, const mg_uint128 *op2, /*out*/mg_uint128 *ret)
{
	unsigned char c;

	c = 0;
	c = mg_uint32_add(c, op1->word[0], op2->word[0], &ret->word[0]);
	c = mg_uint32_add(c, op1->word[1], op2->word[1], &ret->word[1]);
	c = mg_uint32_add(c, op1->word[2], op2->word[2], &ret->word[2]);
	c = mg_uint32_add(c, op1->word[3], op2->word[3], &ret->word[3]);

	return c;
}

static inline int mg_uint128_add_1(/*inout*/mg_uint128 *op1, const mg_uint128 *op2)
{
	return mg_uint128_add(op1, op2, /*out*/op1);
}

static inline int mg_uint128_sub(const mg_uint128 *op1, const mg_uint128 *op2, /*out*/mg_uint128 *ret)
{
	unsigned char b;

	b = 0;
	b = mg_uint32_sub(b, op1->word[0], op2->word[0], &ret->word[0]);
	b = mg_uint32_sub(b, op1->word[1], op2->word[1], &ret->word[1]);
	b = mg_uint32_sub(b, op1->word[2], op2->word[2], &ret->word[2]);
	b = mg_uint32_sub(b, op1->word[3], op2->word[3], &ret->word[3]);

	return b;
}

static inline int mg_uint128_sub_1(/*inout*/mg_uint128 *op1, const mg_uint128 *op2)
{
	return mg_uint128_sub(op1, op2, /*out*/op1);
}

static inline void mg_uint128_neg(const mg_uint128 *op1, mg_uint128 *ret)
{
	unsigned char c;

	ret->word[0] = ~op1->word[0];
	ret->word[1] = ~op1->word[1];
	ret->word[2] = ~op1->word[2];
	ret->word[3] = ~op1->word[3];

	c = 0;
	c = mg_uint32_add(c, ret->word[0], 1, &ret->word[0]);
	c = mg_uint32_add(c, ret->word[1], 0, &ret->word[1]);
	c = mg_uint32_add(c, ret->word[2], 0, &ret->word[2]);
	c = mg_uint32_add(c, ret->word[3], 0, &ret->word[3]);
}

static inline void mg_uint128_neg_1(/*inout*/mg_uint128 *op1)
{
	mg_uint128_neg(op1, /*out*/op1);
}

static inline void mg_uint128_mul_1(
		const mg_uint128 *op1, const mg_uint128 *op2, /*out*/mg_uint128 *_lo, /*out*/mg_uint128 *_hi)
{
	int op1_digits = MG_UINT128_SIZE;
	while(op1_digits > 0 && op1->word[op1_digits-1] == 0)
		op1_digits--;

	int op2_digits = MG_UINT128_SIZE;
	while(op2_digits > 0 && op2->word[op2_digits-1] == 0)
		op2_digits--;

	mg_uint128_mul_digits_1(op1, op1_digits, op2, op2_digits, /*out*/_lo, /*out*/_hi);
}

static inline int mg_uint128_mul(const mg_uint128 *op1, const mg_uint128 *op2, /*out*/mg_uint128 *ret)
{
	int op1_digits = MG_UINT128_SIZE;
	while(op1_digits > 0 && op1->word[op1_digits-1] == 0)
		op1_digits--;

	int op2_digits = MG_UINT128_SIZE;
	while(op2_digits > 0 && op2->word[op2_digits-1] == 0)
		op2_digits--;

	return mg_uint128_mul_digits(op1, op1_digits, op2, op2_digits, /*out*/ret);
}

static inline void mg_uint128_and(const mg_uint128 *op1, const mg_uint128 *op2, /*out*/mg_uint128 *ret)
{
	ret->word[0] = op1->word[0] & op2->word[0];
	ret->word[1] = op1->word[1] & op2->word[1];
	ret->word[2] = op1->word[2] & op2->word[2];
	ret->word[3] = op1->word[3] & op2->word[3];
}

static inline void mg_uint128_and_1(/*inout*/mg_uint128 *op1, const mg_uint128 *op2)
{
	mg_uint128_and(op1, op2, /*out*/op1);
}

static inline void mg_uint128_or(const mg_uint128 *op1, const mg_uint128 *op2, /*out*/mg_uint128 *ret)
{
	ret->word[0] = op1->word[0] | op2->word[0];
	ret->word[1] = op1->word[1] | op2->word[1];
	ret->word[2] = op1->word[2] | op2->word[2];
	ret->word[3] = op1->word[3] | op2->word[3];
}

static inline void mg_uint128_or_1(/*inout*/mg_uint128 *op1, const mg_uint128 *op2)
{
	mg_uint128_or(op1, op2, /*out*/op1);
}

static inline void mg_uint128_xor(const mg_uint128 *op1, const mg_uint128 *op2, /*out*/mg_uint128 *ret)
{
	ret->word[0] = op1->word[0] ^ op2->word[0];
	ret->word[1] = op1->word[1] ^ op2->word[1];
	ret->word[2] = op1->word[2] ^ op2->word[2];
	ret->word[3] = op1->word[3] ^ op2->word[3];
}

static inline void mg_uint128_xor_1(/*inout*/mg_uint128 *op1, const mg_uint128 *op2)
{
	mg_uint128_xor(op1, op2, /*out*/op1);
}

static inline void mg_uint128_not(const mg_uint128 *op1, /*out*/mg_uint128 *ret)
{
	ret->word[0] = ~op1->word[0];
	ret->word[1] = ~op1->word[1];
	ret->word[2] = ~op1->word[2];
	ret->word[3] = ~op1->word[3];
}

static inline void mg_uint128_not_1(/*inout*/mg_uint128 *op1)
{
	mg_uint128_not(op1, /*out*/op1);
}

static inline void mg_uint128_left_shift(const mg_uint128 *op1, int shift, /*out*/mg_uint128 *ret)
{
	uint32_t buf[MG_UINT128_SIZE * 2] = { 0 };

	int words = shift / MG_UINT128_WORD_BITS;
	int bits = shift % MG_UINT128_WORD_BITS;

	if (bits != 0) {
		buf[0 + words] = (op1->word[0] << bits);
		buf[1 + words] = (op1->word[0] >> (MG_UINT128_WORD_BITS - bits)) | (op1->word[1] << bits);
		buf[2 + words] = (op1->word[1] >> (MG_UINT128_WORD_BITS - bits)) | (op1->word[2] << bits);
		buf[3 + words] = (op1->word[2] >> (MG_UINT128_WORD_BITS - bits)) | (op1->word[3] << bits);
		buf[4 + words] = (op1->word[3] >> (MG_UINT128_WORD_BITS - bits));
	}
	else {
		buf[0 + words] = op1->word[0];
		buf[1 + words] = op1->word[1];
		buf[2 + words] = op1->word[2];
		buf[3 + words] = op1->word[3];
	}

	ret->word[0] = buf[0];
	ret->word[1] = buf[1];
	ret->word[2] = buf[2];
	ret->word[3] = buf[3];
}

static inline void mg_uint128_left_shift_1(/*inout*/mg_uint128 *op1, int shift)
{
	mg_uint128_left_shift(op1, shift, op1);
}

static inline void mg_uint128_left_shift_small(const mg_uint128 *op1, int shift, /*out*/mg_uint128 *ret)
{
	ret->word[0] = (op1->word[0] << shift);
	ret->word[1] = (op1->word[1] << shift) | (op1->word[0] >> (MG_UINT128_WORD_BITS - shift));
	ret->word[2] = (op1->word[2] << shift) | (op1->word[1] >> (MG_UINT128_WORD_BITS - shift));
	ret->word[3] = (op1->word[3] << shift) | (op1->word[2] >> (MG_UINT128_WORD_BITS - shift));
}

static inline void mg_uint128_left_shift_small_1(/*inout*/mg_uint128 *op1, int shift)
{
	mg_uint128 ret;

	mg_uint128_left_shift_small(op1, shift, &ret);

	*op1 = ret;
}

static inline void mg_uint128_right_shift(const mg_uint128 *op1, int shift, /*out*/mg_uint128 *ret)
{
	uint32_t buf[MG_UINT128_SIZE * 2] = { 0 };

	int words = shift / MG_UINT128_WORD_BITS;
	int bits = shift % MG_UINT128_WORD_BITS;

	if (bits != 0) {
		buf[4 - words] = (op1->word[0] >> bits) | (op1->word[1] << (MG_UINT128_WORD_BITS - bits));
		buf[5 - words] = (op1->word[1] >> bits) | (op1->word[2] << (MG_UINT128_WORD_BITS - bits));
		buf[6 - words] = (op1->word[2] >> bits) | (op1->word[3] << (MG_UINT128_WORD_BITS - bits));
		buf[7 - words] = (op1->word[3] >> bits);
	}
	else {
		buf[4 - words] = op1->word[0];
		buf[5 - words] = op1->word[1];
		buf[6 - words] = op1->word[2];
		buf[7 - words] = op1->word[3];
	}

	ret->word[0] = buf[4];
	ret->word[1] = buf[5];
	ret->word[2] = buf[6];
	ret->word[3] = buf[7];
}

static inline void mg_uint128_right_shift_1(/*inout*/mg_uint128 *op1, int shift)
{
	mg_uint128_right_shift(op1, shift, op1);
}

static inline void mg_uint128_right_shift_small(const mg_uint128 *op1, int shift, mg_uint128 *ret)
{
	ret->word[0] = (op1->word[0] >> shift) | (op1->word[1] << (MG_UINT128_WORD_BITS - shift));
	ret->word[1] = (op1->word[1] >> shift) | (op1->word[2] << (MG_UINT128_WORD_BITS - shift));
	ret->word[2] = (op1->word[2] >> shift) | (op1->word[3] << (MG_UINT128_WORD_BITS - shift));
	ret->word[3] = (op1->word[3] >> shift);
}

static inline void mg_uint128_right_shift_small_1(/*inout*/mg_uint128 *op1, int shift)
{
	mg_uint128 ret;

	mg_uint128_right_shift_small(op1, shift, &ret);

	*op1 = ret;
}

static inline void mg_uint128_get_bits(/*inout*/mg_uint128 *op1, int op2)
{
	int bytes = op2 / MG_UINT128_WORD_BITS;
	int bits = op2 % MG_UINT128_WORD_BITS;

	for(int i = bytes + (bits > 0 ? 1 : 0); i < MG_UINT128_SIZE; i++) {
		op1->word[i] = 0;
	}

	if(bits != 0) {
		op1->word[bytes] = (op1->word[bytes] << (MG_UINT128_WORD_BITS - bits)) >> (MG_UINT128_WORD_BITS - bits);
	}
}

static inline int mg_uint128_get_bit_size(const mg_uint128 *value)
{
	if(value->word[3] != 0) {
		return 3 * MG_UINT128_WORD_BITS + mg_uint32_get_bit_size(value->word[3]);
	}
	if(value->word[2] != 0) {
		return 2 * MG_UINT128_WORD_BITS + mg_uint32_get_bit_size(value->word[2]);
	}
	if(value->word[1] != 0) {
		return 1 * MG_UINT128_WORD_BITS + mg_uint32_get_bit_size(value->word[1]);
	}
	if(value->word[0] != 0) {
		return mg_uint32_get_bit_size(value->word[0]);
	}
	return 0;
}

static inline void mg_uint128_set_bit(mg_uint128 *op1, int bit_index)
{
	int words = bit_index / MG_UINT128_WORD_BITS;
	int bits = bit_index % MG_UINT128_WORD_BITS;

	op1->word[words] |= (uint32_t)1 << (uint32_t)bits;
}

#ifdef __cplusplus
}
#endif
