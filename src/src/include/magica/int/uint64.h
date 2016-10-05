/**
 * Copyright (C) Takuo Hada 2015-2016
 * @author t.hada 2016/08/27
 *
 * 64 bit integer for decimal.
 * this source includes decimal implementation.
 */
#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

static inline int mg_uint64_get_bit_size(uint64_t value)
{
	int n = 0;

	if ((value & 0xFFFFFFFF00000000ULL) != 0) {
		n += 32;
		value >>= 32;
	}
	if ((value & 0xFFFF0000ULL) != 0) {
		n += 16;
		value >>= 16;
	}
	if ((value & 0xFF00ULL) != 0) {
		n += 8;
		value >>= 8;
	}
	if ((value & 0xF0ULL) != 0) {
		n += 4;
		value >>= 4;
	}
	if ((value & 0xCULL) != 0) {
		n += 2;
		value >>= 2;
	}
	if ((value & 0x2ULL) != 0) {
		n += 1;
		value >>= 1;
	}
	if ((value & 0x1ULL) != 0) {
		n += 1;
		value >>= 1;
	}

	return n;
}


#ifdef __cplusplus
}
#endif
	