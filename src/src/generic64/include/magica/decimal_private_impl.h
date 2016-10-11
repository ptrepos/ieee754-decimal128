/**
 * Copyright (C) Takuo Hada 2015-2016
 * @author t.hada 2011/03/15 
 *
 * 128 bit decimal implements.
 */
#pragma once

#include <assert.h>

#include <magica/uint256.h>
#include <magica/decimal_private_def.h>
#include <magica/decimal.h>

#define WORD_COUNT			(sizeof(mg_decimal) / sizeof(uint64_t))
#define INFO_INDEX			(WORD_COUNT-1)

#define SCALE_MASK			(0x7FFE000000000000ULL)
#define SCALE_BITINDEX		(49L)
#define SIGN_MASK			(0x8000000000000000ULL)
#define SIGN_BITINDEX		(63L)
#define FRACTION_MASK		(0x0001FFFFFFFFFFFFULL)
#define FRACTION_BITINDEX	(0L)

#define DECIMAL_STATUS_ZERO		(0)
#define DECIMAL_STATUS_REGULAR	(1)
#define DECIMAL_STATUS_INF		(2)
#define DECIMAL_STATUS_NAN		(3)

static inline int __mg_decimal_is_overflow(const mg_uint256 *value)
{
	return mg_uint256_compare(value, mg_uint256_get_10eN(DIGIT_MAX)) >= 0;
}

MG_PRIVATE mg_decimal_error __mg_decimal_round_down_max_digits(mg_uint256 *value, int scale, int *rounded_scale);

static inline uint64_t __mg_decimal_get_bits(uint64_t dest, uint64_t mask, int bitindex)
{
	return (uint64_t)((dest & mask) >> bitindex);
}

static inline uint64_t __mg_deciaml_set_bits(uint64_t dest, uint64_t value, uint64_t mask, int bitindex)
{
	return (dest & ~mask) | ((value << bitindex) & mask);
}

static inline int __mg_decimal_get_sign(const mg_decimal *value)
{
	return (int)__mg_decimal_get_bits(
		value->w[INFO_INDEX], SIGN_MASK, SIGN_BITINDEX);
}

static inline void __mg_decimal_set_sign(mg_decimal *value, int sign)
{
	value->w[INFO_INDEX] = __mg_deciaml_set_bits(
		value->w[INFO_INDEX],
		sign,
		SIGN_MASK,
		SIGN_BITINDEX);
}


static inline void __mg_decimal_parse(
		const mg_decimal *value,
		int *sign,
		int *scale,
		mg_uint256 *fraction,
		int *status)
{
	*sign = ((value->w[INFO_INDEX] & SIGN_MASK) >> SIGN_BITINDEX);

	int nscale = ((value->w[INFO_INDEX] & SCALE_MASK) >> SCALE_BITINDEX);
	
	fraction->word[0] = value->w[0];
	fraction->word[1] = ((value->w[INFO_INDEX] & FRACTION_MASK) >> FRACTION_BITINDEX);
	fraction->word[2] = 0;
	fraction->word[3] = 0;

	if(nscale == SCALE_INFINITY) {
		if(mg_uint256_is_zero(fraction)) {
			*status = DECIMAL_STATUS_INF;
		} else {
			*status = DECIMAL_STATUS_NAN;
		}
	} else if(nscale == 0) {
		*status = DECIMAL_STATUS_ZERO;
	} else {
		*scale = nscale - SCALE_SIGNEXPAND;
		*status = DECIMAL_STATUS_REGULAR;
	}
}

static inline void __mg_decimal_set(/*out*/mg_decimal *value, int sign, int scale, const mg_uint256 *fraction)
{
	value->w[0] = fraction->word[0];
	value->w[INFO_INDEX] =
		(((uint64_t)sign << SIGN_BITINDEX) & SIGN_MASK) | 
		(((uint64_t)scale << SCALE_BITINDEX) & SCALE_MASK) | 
		(((uint64_t)fraction->word[1] << FRACTION_BITINDEX) & FRACTION_MASK);
}

MG_PRIVATE mg_decimal_error adjust_overflow_scale(
		int sign, 
		int scale, 
		const mg_uint256 *fraction,
		/*out*/int *adjusted_scale,
		/*out*/mg_uint256 *adjusted_fraction,
		/*out*/mg_decimal *value);

static inline mg_decimal_error __mg_decimal_set_1(/*out*/mg_decimal *value, int sign, int scale, const mg_uint256 *fraction)
{
	mg_decimal_error err;

	assert(sign == 0 || sign == 1);

	if(mg_uint256_is_zero(fraction)) {
		mg_decimal_zero(/*out*/value);
		goto _EXIT;
	}
	if(scale < SCALE_MIN) {
		mg_decimal_zero(/*out*/value);
		goto _EXIT;
	}

	mg_uint256 adjusted_fraction;
	if(scale > SCALE_MAX) {
		err = adjust_overflow_scale(sign, scale, fraction,
				/*out*/&scale, /*out*/&adjusted_fraction, /*out*/value);
		if(err != 0)
			goto _ERROR;
	} else {
		adjusted_fraction = *fraction;
	}
	
	assert(__mg_decimal_is_overflow(&adjusted_fraction) == 0);

	scale += SCALE_SIGNEXPAND;

	value->w[0] = adjusted_fraction.word[0];
	value->w[INFO_INDEX] =
		(((uint64_t)sign << SIGN_BITINDEX) & SIGN_MASK) | 
		(((uint64_t)scale << SCALE_BITINDEX) & SCALE_MASK) | 
		(((uint64_t)adjusted_fraction.word[1] << FRACTION_BITINDEX) & FRACTION_MASK);

_EXIT:
	return 0;
_ERROR:
	return err;
}
