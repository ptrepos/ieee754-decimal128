/**
* Copyright (C) Takuo Hada 2015-2016
* @author t.hada 2016/08/27
*
* 256 bit integer for decimal.
*/
#pragma once

#include <assert.h>

#include <magica/int/uint32.h>
#include <magica/int/uint64.h>
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

static inline void mg_uint256_set_zero(mg_uint256 *dest)
{
	dest->word[0] = 0;
	dest->word[1] = 0;
	dest->word[2] = 0;
	dest->word[3] = 0;
	dest->word[4] = 0;
	dest->word[5] = 0;
	dest->word[6] = 0;
	dest->word[7] = 0;
}

static inline void mg_uint256_set(mg_uint256 *dest, uint64_t value)
{
	dest->word[0] = (uint32_t)value;
	dest->word[1] = (uint32_t)(value >> 32);
	dest->word[2] = 0;
	dest->word[3] = 0;
	dest->word[4] = 0;
	dest->word[5] = 0;
	dest->word[6] = 0;
	dest->word[7] = 0;
}

static inline uint32_t mg_uint256_get_uint32(const mg_uint256 *value)
{
	return value->word[0];
}


static inline uint64_t mg_uint256_get_uint64(const mg_uint256 *value)
{
	return (uint64_t)value->word[0] | ((uint64_t)value->word[1] << 32);
}

static inline void mg_uint256_swap(mg_uint256 **a, mg_uint256 **b)
{
	mg_uint256 *tmp = *a;
	*a = *b;
	*b = tmp;
}

static inline int mg_uint256_is_zero(const mg_uint256 *value)
{
	return (value->word[7] | value->word[6] | value->word[5] | value->word[4] |
		    value->word[3] | value->word[2] | value->word[1] | value->word[0]) == 0;
}

static inline int mg_uint256_compare(const mg_uint256 *value1, const mg_uint256 *value2)
{
	if (value1->word[7] != value2->word[7])
		return value1->word[7] < value2->word[7] ? -1 : 1;
	if (value1->word[6] != value2->word[6])
		return value1->word[6] < value2->word[6] ? -1 : 1;
	if (value1->word[5] != value2->word[5])
		return value1->word[5] < value2->word[5] ? -1 : 1;
	if (value1->word[4] != value2->word[4])
		return value1->word[4] < value2->word[4] ? -1 : 1;
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

static inline int mg_uint256_add(const mg_uint256 *op1, const mg_uint256 *op2, /*out*/mg_uint256 *ret)
{
	unsigned char c;

	c = 0;
	c = mg_uint32_add(c, op1->word[0], op2->word[0], &ret->word[0]);
	c = mg_uint32_add(c, op1->word[1], op2->word[1], &ret->word[1]);
	c = mg_uint32_add(c, op1->word[2], op2->word[2], &ret->word[2]);
	c = mg_uint32_add(c, op1->word[3], op2->word[3], &ret->word[3]);
	c = mg_uint32_add(c, op1->word[4], op2->word[4], &ret->word[4]);
	c = mg_uint32_add(c, op1->word[5], op2->word[5], &ret->word[5]);
	c = mg_uint32_add(c, op1->word[6], op2->word[6], &ret->word[6]);
	c = mg_uint32_add(c, op1->word[7], op2->word[7], &ret->word[7]);

	return c;
}

static inline int mg_uint256_add_1(/*inout*/mg_uint256 *op1, const mg_uint256 *op2)
{
	return mg_uint256_add(op1, op2, /*out*/op1);
}

static inline int mg_uint256_add128(const mg_uint256 *op1, const mg_uint256 *op2, /*out*/mg_uint256 *ret)
{
	unsigned char c;

	c = 0;
	c = mg_uint32_add(c, op1->word[0], op2->word[0], &ret->word[0]);
	c = mg_uint32_add(c, op1->word[1], op2->word[1], &ret->word[1]);
	c = mg_uint32_add(c, op1->word[2], op2->word[2], &ret->word[2]);
	c = mg_uint32_add(c, op1->word[3], op2->word[3], &ret->word[3]);

	return c;
}

static inline int mg_uint256_add128_1(/*inout*/mg_uint256 *op1, const mg_uint256 *op2)
{
	return mg_uint256_add128(op1, op2, /*out*/op1);
}

static inline int mg_uint256_sub(const mg_uint256 *op1, const mg_uint256 *op2, /*out*/mg_uint256 *ret)
{
	unsigned char b;

	b = 0;
	b = mg_uint32_sub(b, op1->word[0], op2->word[0], &ret->word[0]);
	b = mg_uint32_sub(b, op1->word[1], op2->word[1], &ret->word[1]);
	b = mg_uint32_sub(b, op1->word[2], op2->word[2], &ret->word[2]);
	b = mg_uint32_sub(b, op1->word[3], op2->word[3], &ret->word[3]);
	b = mg_uint32_sub(b, op1->word[4], op2->word[4], &ret->word[4]);
	b = mg_uint32_sub(b, op1->word[5], op2->word[5], &ret->word[5]);
	b = mg_uint32_sub(b, op1->word[6], op2->word[6], &ret->word[6]);
	b = mg_uint32_sub(b, op1->word[7], op2->word[7], &ret->word[7]);

	return b;
}

static inline int mg_uint256_sub_1(/*inout*/mg_uint256 *op1, const mg_uint256 *op2)
{
	return mg_uint256_sub(op1, op2, /*out*/op1);
}

static inline int mg_uint256_sub128(const mg_uint256 *op1, const mg_uint256 *op2, /*out*/mg_uint256 *ret)
{
	unsigned char b;

	b = 0;
	b = mg_uint32_sub(b, op1->word[0], op2->word[0], &ret->word[0]);
	b = mg_uint32_sub(b, op1->word[1], op2->word[1], &ret->word[1]);
	b = mg_uint32_sub(b, op1->word[2], op2->word[2], &ret->word[2]);
	b = mg_uint32_sub(b, op1->word[3], op2->word[3], &ret->word[3]);

	return b;
}

static inline int mg_uint256_sub128_1(/*inout*/mg_uint256 *op1, const mg_uint256 *op2)
{
	return mg_uint256_sub128(op1, op2, /*out*/op1);
}

static inline void mg_uint256_neg(const mg_uint256 *op1, mg_uint256 *ret)
{
	unsigned char c;

	ret->word[0] = ~op1->word[0];
	ret->word[1] = ~op1->word[1];
	ret->word[2] = ~op1->word[2];
	ret->word[3] = ~op1->word[3];
	ret->word[4] = ~op1->word[4];
	ret->word[5] = ~op1->word[5];
	ret->word[6] = ~op1->word[6];
	ret->word[7] = ~op1->word[7];

	c = 0;
	c = mg_uint32_add(c, ret->word[0], 1, &ret->word[0]);
	c = mg_uint32_add(c, ret->word[1], 0, &ret->word[1]);
	c = mg_uint32_add(c, ret->word[2], 0, &ret->word[2]);
	c = mg_uint32_add(c, ret->word[3], 0, &ret->word[3]);
	c = mg_uint32_add(c, ret->word[4], 0, &ret->word[4]);
	c = mg_uint32_add(c, ret->word[5], 0, &ret->word[5]);
	c = mg_uint32_add(c, ret->word[6], 0, &ret->word[6]);
	c = mg_uint32_add(c, ret->word[7], 0, &ret->word[7]);
}

static inline void mg_uint256_neg_1(/*inout*/mg_uint256 *op1)
{
	mg_uint256_neg(op1, /*out*/op1);
}
	
static inline void mg_uint256_neg128(const mg_uint256 *op1, mg_uint256 *ret)
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

static inline void mg_uint256_neg128_1(/*inout*/mg_uint256 *op1)
{
	mg_uint256_neg128(op1, /*out*/op1);
}

#if 0	// •s—v‚É‚µ‚½‚¢
static inline int mg_uint256_mul256x64(
	const mg_uint256 *op1, 
	const mg_uint256 *op2, 
	/*out*/mg_uint256 *ret)
{
	int op1_digits = MG_UINT256_SIZE;
	while(op1_digits > 0 && op1->word[op1_digits-1] == 0)
		op1_digits--;

	int op2_digits = MG_UINT256_SIZE / 4;
	while(op2_digits > 0 && op2->word[op2_digits-1] == 0)
		op2_digits--;

	return mg_uint256_mul_digits(op1, op1_digits, op2, op2_digits, /*out*/ret);
}
#endif

static inline void mg_uint256_mul128(const mg_uint256 *op1, const mg_uint256 *op2, /*out*/mg_uint256 *ret)
{
	mg_uint256 dummy;

	mg_uint256_mul_digits_1(op1, MG_UINT256_DIGITS_128, op2, MG_UINT256_DIGITS_128, /*out*/ret, /*out*/&dummy);
}

static inline void mg_uint256_mul_1(
	const mg_uint256 *op1, const mg_uint256 *op2, /*out*/mg_uint256 *low, /*out*/mg_uint256 *high)
{
	int op1_digits = MG_UINT256_SIZE;
	while(op1_digits > 0 && op1->word[op1_digits-1] == 0)
		op1_digits--;

	int op2_digits = MG_UINT256_SIZE;
	while(op2_digits > 0 && op2->word[op2_digits-1] == 0)
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
	ret->word[4] = op1->word[4] & op2->word[4];
	ret->word[5] = op1->word[5] & op2->word[5];
	ret->word[6] = op1->word[6] & op2->word[6];
	ret->word[7] = op1->word[7] & op2->word[7];
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
	ret->word[4] = op1->word[4] | op2->word[4];
	ret->word[5] = op1->word[5] | op2->word[5];
	ret->word[6] = op1->word[6] | op2->word[6];
	ret->word[7] = op1->word[7] | op2->word[7];
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
	ret->word[4] = op1->word[4] ^ op2->word[4];
	ret->word[5] = op1->word[5] ^ op2->word[5];
	ret->word[6] = op1->word[6] ^ op2->word[6];
	ret->word[7] = op1->word[7] ^ op2->word[7];
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
	ret->word[4] = ~op1->word[4];
	ret->word[5] = ~op1->word[5];
	ret->word[6] = ~op1->word[6];
	ret->word[7] = ~op1->word[7];
}

static inline void mg_uint256_not_1(/*inout*/mg_uint256 *op1)
{
	mg_uint256_not(op1, /*out*/op1);
}

static inline void mg_uint256_left_shift(const mg_uint256 *op1, int shift, /*out*/mg_uint256 *ret)
{
	uint32_t buf[MG_UINT256_SIZE * 2] = { 0 };

	int words = shift / MG_UINT256_WORD_BITS;
	int bits = shift % MG_UINT256_WORD_BITS;

	if (bits != 0) {
		buf[0 + words] = (op1->word[0] << bits);
		buf[1 + words] = (op1->word[0] >> (MG_UINT256_WORD_BITS - bits)) | (op1->word[1] << bits);
		buf[2 + words] = (op1->word[1] >> (MG_UINT256_WORD_BITS - bits)) | (op1->word[2] << bits);
		buf[3 + words] = (op1->word[2] >> (MG_UINT256_WORD_BITS - bits)) | (op1->word[3] << bits);
		buf[4 + words] = (op1->word[3] >> (MG_UINT256_WORD_BITS - bits)) | (op1->word[4] << bits);
		buf[5 + words] = (op1->word[4] >> (MG_UINT256_WORD_BITS - bits)) | (op1->word[5] << bits);
		buf[6 + words] = (op1->word[5] >> (MG_UINT256_WORD_BITS - bits)) | (op1->word[6] << bits);
		buf[7 + words] = (op1->word[6] >> (MG_UINT256_WORD_BITS - bits)) | (op1->word[7] << bits);
	}
	else {
		buf[0 + words] = op1->word[0];
		buf[1 + words] = op1->word[1];
		buf[2 + words] = op1->word[2];
		buf[3 + words] = op1->word[3];
		buf[4 + words] = op1->word[4];
		buf[5 + words] = op1->word[5];
		buf[6 + words] = op1->word[6];
		buf[7 + words] = op1->word[7];
	}

	ret->word[0] = buf[0];
	ret->word[1] = buf[1];
	ret->word[2] = buf[2];
	ret->word[3] = buf[3];
	ret->word[4] = buf[4];
	ret->word[5] = buf[5];
	ret->word[6] = buf[6];
	ret->word[7] = buf[7];
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
	ret->word[4] = (op1->word[4] << shift) | (op1->word[3] >> (MG_UINT256_WORD_BITS - shift));
	ret->word[5] = (op1->word[5] << shift) | (op1->word[4] >> (MG_UINT256_WORD_BITS - shift));
	ret->word[6] = (op1->word[6] << shift) | (op1->word[5] >> (MG_UINT256_WORD_BITS - shift));
	ret->word[7] = (op1->word[7] << shift) | (op1->word[6] >> (MG_UINT256_WORD_BITS - shift));
}

static inline void mg_uint256_left_shift_small_1(/*inout*/mg_uint256 *op1, int shift)
{
	mg_uint256 ret;

	mg_uint256_left_shift_small(op1, shift, &ret);

	*op1 = ret;
}

static inline void mg_uint256_right_shift(const mg_uint256 *op1, int shift, /*out*/mg_uint256 *ret)
{
	uint32_t buf[MG_UINT256_SIZE * 2] = { 0 };

	int words = shift / MG_UINT256_WORD_BITS;
	int bits = shift % MG_UINT256_WORD_BITS;

	if (bits != 0) {
		buf[8 - words] = (op1->word[0] >> bits) | (op1->word[1] << (MG_UINT256_WORD_BITS - bits));
		buf[9 - words] = (op1->word[1] >> bits) | (op1->word[2] << (MG_UINT256_WORD_BITS - bits));
		buf[10 - words] = (op1->word[2] >> bits) | (op1->word[3] << (MG_UINT256_WORD_BITS - bits));
		buf[11 - words] = (op1->word[3] >> bits) | (op1->word[4] << (MG_UINT256_WORD_BITS - bits));
		buf[12 - words] = (op1->word[4] >> bits) | (op1->word[5] << (MG_UINT256_WORD_BITS - bits));
		buf[13 - words] = (op1->word[5] >> bits) | (op1->word[6] << (MG_UINT256_WORD_BITS - bits));
		buf[14 - words] = (op1->word[6] >> bits) | (op1->word[7] << (MG_UINT256_WORD_BITS - bits));
		buf[15 - words] = (op1->word[7] >> bits);
	}
	else {
		buf[8 - words] = op1->word[0];
		buf[9 - words] = op1->word[1];
		buf[10 - words] = op1->word[2];
		buf[11 - words] = op1->word[3];
		buf[12 - words] = op1->word[4];
		buf[13 - words] = op1->word[5];
		buf[14 - words] = op1->word[6];
		buf[15 - words] = op1->word[7];
	}

	ret->word[0] = buf[8];
	ret->word[1] = buf[9];
	ret->word[2] = buf[10];
	ret->word[3] = buf[11];
	ret->word[4] = buf[12];
	ret->word[5] = buf[13];
	ret->word[6] = buf[14];
	ret->word[7] = buf[15];
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
	ret->word[3] = (op1->word[3] >> shift) | (op1->word[4] << (MG_UINT256_WORD_BITS - shift));
	ret->word[4] = (op1->word[4] >> shift) | (op1->word[5] << (MG_UINT256_WORD_BITS - shift));
	ret->word[5] = (op1->word[5] >> shift) | (op1->word[6] << (MG_UINT256_WORD_BITS - shift));
	ret->word[6] = (op1->word[6] >> shift) | (op1->word[7] << (MG_UINT256_WORD_BITS - shift));
	ret->word[7] = (op1->word[7] >> shift);
}

static inline void mg_uint256_right_shift_small_1(/*inout*/mg_uint256 *op1, int shift)
{
	mg_uint256 ret;

	mg_uint256_right_shift_small(op1, shift, &ret);

	*op1 = ret;
}

static inline int mg_uint256_get_bit_size(const mg_uint256 *value)
{
	if(value->word[7] != 0) {
		return 7 * MG_UINT256_WORD_BITS + mg_uint32_get_bit_size(value->word[7]);
	}
	if(value->word[6] != 0) {
		return 6 * MG_UINT256_WORD_BITS + mg_uint32_get_bit_size(value->word[6]);
	}
	if(value->word[5] != 0) {
		return 5 * MG_UINT256_WORD_BITS + mg_uint32_get_bit_size(value->word[5]);
	}
	if(value->word[4] != 0) {
		return 4 * MG_UINT256_WORD_BITS + mg_uint32_get_bit_size(value->word[4]);
	}
	if(value->word[3] != 0) {
		return 3 * MG_UINT256_WORD_BITS + mg_uint32_get_bit_size(value->word[3]);
	}
	if(value->word[2] != 0) {
		return 2 * MG_UINT256_WORD_BITS + mg_uint32_get_bit_size(value->word[2]);
	}
	if(value->word[1] != 0) {
		return 1 * MG_UINT256_WORD_BITS + mg_uint32_get_bit_size(value->word[1]);
	}
	if(value->word[0] != 0) {
		return mg_uint32_get_bit_size(value->word[0]);
	}
	return 0;
}

static inline void mg_uint256_set_bit(mg_uint256 *op1, int bit_index)
{
	int words = bit_index / MG_UINT256_WORD_BITS;
	int bits = bit_index % MG_UINT256_WORD_BITS;

	op1->word[words] |= (uint32_t)1 << (uint32_t)bits;
}

extern MG_PRIVATE const mg_uint256 *MG_UINT256_10eN_TABLE[];

static inline const mg_uint256 *mg_uint256_get_10eN(int digits)
{
	assert(0 <= digits && digits < 78);

	return MG_UINT256_10eN_TABLE[digits];
}

int mg_uint256_get_digits(const mg_uint256 *value);
	
#ifdef __cplusplus
}
#endif
