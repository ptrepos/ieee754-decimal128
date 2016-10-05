/**
 * Copyright (C) Takuo Hada 2015-2016
 * @author t.hada 2016/07/09
 *
 * 256 bit integer for decimal.
 * this source includes decimal implementation.
 */
#pragma once

#include <assert.h>

#include <magica/int/uint64.h>
#include <magica/int/uint128_def.h>
#include <magica/int/uint256_def.h>
#include <magica/int/uint128_impl.h>

#include "intop.h"

#ifdef __cplusplus
extern "C" {
#endif

int mg_uint256_mul_digits(const mg_uint256 *op1, int op1_digits, const mg_uint256 *op2, int op2_digits, /*out*/mg_uint256 *ret);
void mg_uint256_mul_digits_1(const mg_uint256 *op1, int op1_digits, const mg_uint256 *op2, int op2_digits, /*out*/mg_uint256 *low, /*out*/mg_uint256 *high);

int mg_uint256_div(
		const mg_uint256 *op1, 
		const mg_uint256 *op2, 
		/*out*/mg_uint256 *quotient, 
		/*out*/mg_uint256 *reminder);

int mg_uint256_div_maclaurin(
		const mg_uint256 *op1,
		const mg_uint256 *op2,
		/*out*/mg_uint256 *quotient,
		/*out*/mg_uint256 *reminder);

void mg_uint256_test_to_string(const mg_uint256 *value, char *buf);
void mg_uint256_test_convert(const char *buf, mg_uint256 *value);
void mg_uint256_test_to_hex_string(const mg_uint256 *value, char *buf);
void mg_uint256_test_hex_convert(const char *buf, mg_uint256 *value);

static inline void mg_uint256_set_zero(/*out*/mg_uint256 *dest)
{
	dest->word[0] = 0;
	dest->word[1] = 0;
	dest->word[2] = 0;
	dest->word[3] = 0;
}

static inline void mg_uint256_set(/*out*/mg_uint256 *dest, uint64_t value)
{
	dest->word[0] = value;
	dest->word[1] = 0;
	dest->word[2] = 0;
	dest->word[3] = 0;
}

static inline void mg_uint256_set128(/*out*/mg_uint256 *dest, const mg_uint128 *value)
{
	dest->word[0] = value->word[0];
	dest->word[1] = value->word[1];
	dest->word[2] = 0;
	dest->word[3] = 0;
}

static inline uint32_t mg_uint256_get_uint32(const mg_uint256 *value)
{
	return (uint32_t)value->word[0];
}

static inline uint64_t mg_uint256_get_uint64(const mg_uint256 *value)
{
	return value->word[0];
}

static inline void mg_uint256_get_uint128(const mg_uint256 *value, mg_uint128 *ret)
{
	ret->word[0] = value->word[0];
	ret->word[1] = value->word[1];
}

static inline void mg_uint256_swap(mg_uint256 **a, mg_uint256 **b)
{
	mg_uint256 *tmp = *a;
	*a = *b;
	*b = tmp;
}

static inline int mg_uint256_is_zero(const mg_uint256 *value)
{
	return (value->word[3] | value->word[2] | value->word[1] | value->word[0]) == 0;
}

static inline int mg_uint256_compare(const mg_uint256 *value1, const mg_uint256 *value2)
{
	if(value1->word[3] != value2->word[3]) 
		return value1->word[3] < value2->word[3] ? -1: 1;
	if(value1->word[2] != value2->word[2]) 
		return value1->word[2] < value2->word[2] ? -1: 1;
	if (value1->word[1] != value2->word[1])
		return value1->word[1] < value2->word[1] ? -1 : 1;
	if (value1->word[0] != value2->word[0])
		return value1->word[0] < value2->word[0] ? -1 : 1;
	return 0;
}

static inline int mg_uint256_add(const mg_uint256 *op1, const mg_uint256 *op2, /*out*/mg_uint256 *ret)
{
	unsigned char c;
	
	c = 0;
	c = mg_uint64_add(c, op1->word[0], op2->word[0], &ret->word[0]);
	c = mg_uint64_add(c, op1->word[1], op2->word[1], &ret->word[1]);
	c = mg_uint64_add(c, op1->word[2], op2->word[2], &ret->word[2]);
	c = mg_uint64_add(c, op1->word[3], op2->word[3], &ret->word[3]);

	return c != 0;
}

static inline int mg_uint256_add_1(/*inout*/mg_uint256 *op1, const mg_uint256 *op2)
{
	return mg_uint256_add(op1, op2, /*out*/op1);
}

static inline int mg_uint256_add128(const mg_uint256 *op1, const mg_uint256 *op2, /*out*/mg_uint256 *ret)
{
	unsigned char c;
	
	c = 0;
	c = mg_uint64_add(c, op1->word[0], op2->word[0], &ret->word[0]);
	c = mg_uint64_add(c, op1->word[1], op2->word[1], &ret->word[1]);

	return c != 0;
}

static inline int mg_uint256_add128_1(/*inout*/mg_uint256 *op1, const mg_uint256 *op2)
{
	return mg_uint256_add128(op1, op2, /*out*/op1);
}

static inline int mg_uint256_sub(const mg_uint256 *op1, const mg_uint256 *op2, /*out*/mg_uint256 *ret)
{
	unsigned char b;

	b = 0;
	b = mg_uint64_sub(b, op1->word[0], op2->word[0], &ret->word[0]);
	b = mg_uint64_sub(b, op1->word[1], op2->word[1], &ret->word[1]);
	b = mg_uint64_sub(b, op1->word[2], op2->word[2], &ret->word[2]);
	b = mg_uint64_sub(b, op1->word[3], op2->word[3], &ret->word[3]);

	return b != 0;
}

static inline int mg_uint256_sub_1(/*inout*/mg_uint256 *op1, const mg_uint256 *op2)
{
	return mg_uint256_sub(op1, op2, /*out*/op1);
}

static inline int mg_uint256_sub128(const mg_uint256 *op1, const mg_uint256 *op2, /*out*/mg_uint256 *ret)
{
	unsigned char b;

	b = 0;
	b = mg_uint64_sub(b, op1->word[0], op2->word[0], &ret->word[0]);
	b = mg_uint64_sub(b, op1->word[1], op2->word[1], &ret->word[1]);

	return b != 0;
}

static inline int mg_uint256_sub128_1(/*inout*/mg_uint256 *op1, const mg_uint256 *op2)
{
	return mg_uint256_sub128(op1, op2, /*out*/op1);
}

static inline void mg_uint256_neg(const mg_uint256 *op1, /*out*/mg_uint256 *ret)
{
	unsigned char c;
	
	ret->word[0] = ~op1->word[0];
	ret->word[1] = ~op1->word[1];
	ret->word[2] = ~op1->word[2];
	ret->word[3] = ~op1->word[3];

	c = 0;
	c = mg_uint64_add(c, ret->word[0], 1, &ret->word[0]);
	c = mg_uint64_add(c, ret->word[1], 0, &ret->word[1]);
	c = mg_uint64_add(c, ret->word[2], 0, &ret->word[2]);
	c = mg_uint64_add(c, ret->word[3], 0, &ret->word[3]);
}

static inline void mg_uint256_neg_1(/*inout*/mg_uint256 *op1)
{
	mg_uint256_neg(op1, /*out*/op1);
}

static inline void mg_uint256_neg128(const mg_uint256 *op1, /*out*/mg_uint256 *ret)
{
	unsigned char c;
	
	ret->word[0] = ~op1->word[0];
	ret->word[1] = ~op1->word[1];

	c = 0;
	c = mg_uint64_add(c, ret->word[0], 1, &ret->word[0]);
	c = mg_uint64_add(c, ret->word[1], 0, &ret->word[1]);
}

static inline void mg_uint256_neg128_1(/*inout*/mg_uint256 *op1)
{
	mg_uint256_neg128(op1, /*out*/op1);
}

static inline void mg_uint256_mul128(const mg_uint256 *op1, const mg_uint256 *op2, /*out*/mg_uint256 *ret)
{
	unsigned c;
	uint64_t lo, hi;
	uint64_t buf[MG_UINT256_SIZE] = {0};

	lo = mg_uint64_mul(op1->word[0], op2->word[0], &hi);

	buf[0] = lo;
	buf[1] = hi;

	lo = mg_uint64_mul(op1->word[1], op2->word[1], &hi);

	buf[2] = lo;
	buf[3] = hi;

	lo = mg_uint64_mul(op1->word[0], op2->word[1], &hi);

	c = 0;
	c = mg_uint64_add(c, buf[1], lo, &buf[1]);
	c = mg_uint64_add(c, buf[2], hi, &buf[2]);
	c = mg_uint64_add(c, buf[3], 0, &buf[3]);

	lo = mg_uint64_mul(op1->word[1], op2->word[0], &hi);

	c = 0;
	c = mg_uint64_add(c, buf[1], lo, &buf[1]);
	c = mg_uint64_add(c, buf[2], hi, &buf[2]);
	c = mg_uint64_add(c, buf[3], 0, &buf[3]);

	ret->word[0] = buf[0];
	ret->word[1] = buf[1];
	ret->word[2] = buf[2];
	ret->word[3] = buf[3];
}

static inline void mg_uint256_mul_1(const mg_uint256 *op1, const mg_uint256 *op2, /*out*/mg_uint256 *low, /*out*/mg_uint256 *high)
{
	int op1_digits = MG_UINT256_SIZE;
	while (op1_digits > 0 && op1->word[op1_digits - 1] == 0)
		op1_digits--;

	int op2_digits = MG_UINT256_SIZE;
	while (op2_digits > 0 && op2->word[op2_digits - 1] == 0)
		op2_digits--;

	mg_uint256_mul_digits_1(op1, op1_digits, op2, op2_digits, /*out*/low, /*out*/high);
}

static inline int mg_uint256_mul(const mg_uint256 *op1, const mg_uint256 *op2, /*out*/mg_uint256 *ret)
{
	int op1_digits = MG_UINT256_SIZE;
	while(op1_digits > 0 && op1->word[op1_digits-1] == 0)
		op1_digits--;

	int op2_digits = MG_UINT256_SIZE;
	while(op2_digits > 0 && op2->word[op2_digits-1] == 0)
		op2_digits--;

	return mg_uint256_mul_digits(op1, op1_digits, op2, op2_digits, /*out*/ret);
}

static inline int mg_uint256_div_1(/*inout*/mg_uint256 *op1, const mg_uint256 *op2, /*out*/mg_uint256 *quotient)
{
	return mg_uint256_div(op1, op2, /*out*/quotient, /*out*/op1);
}

static inline void mg_uint256_and(const mg_uint256 *op1, const mg_uint256 *op2, /*out*/mg_uint256 *ret)
{
	ret->word[0] = op1->word[0] & op2->word[0];
	ret->word[1] = op1->word[1] & op2->word[1];
	ret->word[2] = op1->word[2] & op2->word[2];
	ret->word[3] = op1->word[3] & op2->word[3];
}

static inline void mg_uint256_and_1(/*inout*/mg_uint256 *op1, const mg_uint256 *op2)
{
	mg_uint256_and(op1, op2, /*out*/op1);
}

static inline void mg_uint256_or(const mg_uint256 *op1, const mg_uint256 *op2, /*out*/mg_uint256 *ret)
{
	ret->word[0] = op1->word[0] | op2->word[0];
	ret->word[1] = op1->word[1] | op2->word[1];
	ret->word[2] = op1->word[2] | op2->word[2];
	ret->word[3] = op1->word[3] | op2->word[3];
}

static inline void mg_uint256_or_1(/*inout*/mg_uint256 *op1, const mg_uint256 *op2)
{
	mg_uint256_or(op1, op2, /*out*/op1);
}

static inline void mg_uint256_xor(const mg_uint256 *op1, const mg_uint256 *op2, /*out*/mg_uint256 *ret)
{
	ret->word[0] = op1->word[0] ^ op2->word[0];
	ret->word[1] = op1->word[1] ^ op2->word[1];
	ret->word[2] = op1->word[2] ^ op2->word[2];
	ret->word[3] = op1->word[3] ^ op2->word[3];
}

static inline void mg_uint256_xor_1(/*inout*/mg_uint256 *op1, const mg_uint256 *op2)
{
	mg_uint256_xor(op1, op2, /*out*/op1);
}

static inline void mg_uint256_not(const mg_uint256 *op1, /*out*/mg_uint256 *ret)
{
	ret->word[0] = ~op1->word[0];
	ret->word[1] = ~op1->word[1];
	ret->word[2] = ~op1->word[2];
	ret->word[3] = ~op1->word[3];
}

static inline void mg_uint256_not_1(/*inout*/mg_uint256 *op1)
{
	mg_uint256_not(op1, /*out*/op1);
}

static inline void mg_uint256_left_shift(const mg_uint256 *op1, int shift, /*out*/mg_uint256 *ret)
{
	uint64_t buf[MG_UINT256_SIZE * 2] = { 0 };

	int words = shift / MG_UINT256_WORD_BITS;
	int bits = shift % MG_UINT256_WORD_BITS;

	if (bits != 0) {
		buf[0 + words] = (op1->word[0] << bits);
		buf[1 + words] = (op1->word[0] >> (MG_UINT256_WORD_BITS - bits)) | (op1->word[1] << bits);
		buf[2 + words] = (op1->word[1] >> (MG_UINT256_WORD_BITS - bits)) | (op1->word[2] << bits);
		buf[3 + words] = (op1->word[2] >> (MG_UINT256_WORD_BITS - bits)) | (op1->word[3] << bits);
		buf[4 + words] = (op1->word[3] >> (MG_UINT256_WORD_BITS - bits));
	} else {
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

static inline void mg_uint256_left_shift_1(/*inout*/mg_uint256 *op1, int shift)
{
	mg_uint256_left_shift(op1, shift, op1);
}

static inline void mg_uint256_left_shift_small(const mg_uint256 *op1, int shift, /*out*/mg_uint256 *ret)
{
	ret->word[0] = (op1->word[0] << shift);
	ret->word[1] = (op1->word[1] << shift) | (op1->word[0] >> (MG_UINT256_WORD_BITS - shift));
	ret->word[2] = (op1->word[2] << shift) | (op1->word[1] >> (MG_UINT256_WORD_BITS - shift));
	ret->word[3] = (op1->word[3] << shift) | (op1->word[2] >> (MG_UINT256_WORD_BITS - shift));
}

static inline void mg_uint256_left_shift_small_1(/*inout*/mg_uint256 *op1, int shift)
{
	mg_uint256 ret;

	mg_uint256_left_shift_small(op1, shift, &ret);

	*op1 = ret;
}

static inline void mg_uint256_right_shift(const mg_uint256 *op1, int shift, mg_uint256 *ret)
{
	uint64_t buf[MG_UINT256_SIZE * 2] = { 0 };

	int words = shift / MG_UINT256_WORD_BITS;
	int bits = shift % MG_UINT256_WORD_BITS;

	if (bits != 0) {
		buf[4 - words] = (op1->word[0] >> bits) | (op1->word[1] << (MG_UINT256_WORD_BITS - shift));
		buf[5 - words] = (op1->word[1] >> bits) | (op1->word[2] << (MG_UINT256_WORD_BITS - bits));
		buf[6 - words] = (op1->word[2] >> bits) | (op1->word[3] << (MG_UINT256_WORD_BITS - bits));
		buf[7 - words] = (op1->word[3] >> bits);
	} else {
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

static inline void mg_uint256_right_shift_1(/*inout*/mg_uint256 *op1, int shift)
{
	mg_uint256_right_shift(op1, shift, op1);
}

static inline void mg_uint256_right_shift_small(const mg_uint256 *op1, int shift, mg_uint256 *ret)
{
	ret->word[0] = (op1->word[0] >> shift) | (op1->word[1] << (MG_UINT256_WORD_BITS - shift));
	ret->word[1] = (op1->word[1] >> shift) | (op1->word[2] << (MG_UINT256_WORD_BITS - shift));
	ret->word[2] = (op1->word[2] >> shift) | (op1->word[3] << (MG_UINT256_WORD_BITS - shift));
	ret->word[3] = (op1->word[3] >> shift);
}

static inline void mg_uint256_right_shift_small_1(/*inout*/mg_uint256 *op1, int shift)
{
	mg_uint256 ret;

	mg_uint256_right_shift_small(op1, shift, &ret);

	*op1 = ret;
}

static inline void mg_uint256_get_bits(/*inout*/mg_uint256 *op1, int op2)
{
	int bytes = op2 / MG_UINT256_WORD_BITS;
	int bits = op2 % MG_UINT256_WORD_BITS;

	for(int i = bytes + (bits > 0 ? 1 : 0); i < MG_UINT256_SIZE; i++) {
		op1->word[i] = 0;
	}

	if(bits != 0) {
		op1->word[bytes] = (op1->word[bytes] << (MG_UINT256_WORD_BITS - bits)) >> (MG_UINT256_WORD_BITS - bits);
	}
}

static inline int mg_uint256_get_bit_size(const mg_uint256 *value)
{
	if(value->word[3] != 0) {
		return 3 * MG_UINT256_WORD_BITS + mg_uint64_get_bit_size(value->word[3]);
	}
	if(value->word[2] != 0) {
		return 2 * MG_UINT256_WORD_BITS + mg_uint64_get_bit_size(value->word[2]);
	}
	if(value->word[1] != 0) {
		return 1 * MG_UINT256_WORD_BITS + mg_uint64_get_bit_size(value->word[1]);
	}
	if(value->word[0] != 0) {
		return mg_uint64_get_bit_size(value->word[0]);
	}
	return 0;
}

static inline void mg_uint256_set_bit(mg_uint256 *op1, int bit)
{
	int words = bit / MG_UINT256_WORD_BITS;
	int bits = bit % MG_UINT256_WORD_BITS;

	op1->word[words] |= (uint64_t)1ULL << (uint64_t)bits;
}

extern const mg_uint256 *MG_UINT256_10eN_TABLE[];

static inline const mg_uint256 *mg_uint256_get_10eN(int digits)
{
	assert(0 <= digits && digits < 78);

	return MG_UINT256_10eN_TABLE[digits];
}

int mg_uint256_get_digits(const mg_uint256 *value);

#ifdef __cplusplus
}
#endif