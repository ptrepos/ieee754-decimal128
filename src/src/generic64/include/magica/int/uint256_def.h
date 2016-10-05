/**
 * Copyright (C) Takuo Hada 2015-2016
 * @author t.hada 2016/07/09
 *
 * 256 bit integer implementation
 */
#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
	
#define MG_UINT256_SIZE	(4)
#define MG_UINT256_WORD_BITS	(64)

/**
 * 256bit integer definition for 64bit.
 */
typedef struct mg_uint256
{
	uint64_t word[MG_UINT256_SIZE];
} mg_uint256;

#define MG_UINT256_DIGITS_32	(1)
#define MG_UINT256_DIGITS_64	(1)
#define MG_UINT256_DIGITS_128	(2)
#define MG_UINT256_DIGITS_192	(3)
#define MG_UINT256_DIGITS_256	(4)

#ifdef __cplusplus
}
#endif