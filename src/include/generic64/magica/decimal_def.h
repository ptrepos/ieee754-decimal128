/**
 * Copyright (C) Takuo Hada 2015-2016
 * @author t.hada 2011/03/15 
 * 
 * 128 bit decimal implements.
 */
#pragma once

#include<stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct mg_decimal
{
	uint64_t w[2];
} mg_decimal;
#ifdef __cplusplus
}
#endif
