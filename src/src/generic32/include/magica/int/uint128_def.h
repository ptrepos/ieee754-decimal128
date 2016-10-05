/**
 * Copyright (C) Takuo Hada 2015-2016
 * @author t.hada 2016/08/27
 *
 * 128 bit integer implementation
 */
#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


#define MG_UINT128_SIZE	(4)
#define MG_UINT128_WORD_BITS	(32)

/**
 * 128bit integer definition for 32bit.
 */
typedef struct mg_uint128
{
	uint32_t word[MG_UINT128_SIZE];
} mg_uint128;

#define MG_UINT128_DIGITS_32	(1)
#define MG_UINT128_DIGITS_64	(2)
#define MG_UINT128_DIGITS_128	(4)

#ifdef __cplusplus
}
#endif
