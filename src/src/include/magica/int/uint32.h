/**
 * Copyright (C) Takuo Hada 2015-2016
 * @author t.hada 2016/08/27
 *
 * 32 bit integer for decimal.
 */
#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
	
static inline int mg_uint32_get_bit_size(uint32_t value)
{
	int n = 0;

	if ((value & 0xFFFF0000) != 0) {
		n += 16;
		value >>= 16;
	}
	if ((value & 0xFF00) != 0) {
		n += 8;
		value >>= 8;
	}
	if ((value & 0xF0) != 0) {
		n += 4;
		value >>= 4;
	}
	if ((value & 0xC) != 0) {
		n += 2;
		value >>= 2;
	}
	if ((value & 0x2) != 0) {
		n += 1;
		value >>= 1;
	}
	if ((value & 0x1) != 0) {
		n += 1;
		value >>= 1;
	}

	return n;
}
	
#ifdef __cplusplus
}
#endif
