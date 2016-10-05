#pragma once

#include <stdio.h>
#include <stdlib.h>

#include <time.h>

#define MG_TEST_BEGIN() { \
	clock_t __test_tm = clock(); \
	printf("BEGIN TEST %s()\n", __func__);

#define MG_TEST_END() \
	printf("END TEST %s() [OK] ELAPSED TIME: %fms\n", \
		__func__, \
		((float)(clock() - __test_tm) / CLOCKS_PER_SEC * 1000.0)); \
}

#define MG_ASSERT_MESSAGE(expression, message) \
{ \
	if(!(expression)) { \
		printf("ERROR: %s FILE: %s:%d", message, __FILE__, __LINE__); \
		abort(); \
	} \
}

#define MG_ASSERT(expression) \
	MG_ASSERT_MESSAGE(expression, #expression)

#define MG_FAIL() \
	MG_ASSERT_MESSAGE(0, "fail")

#define MG_OK(expression) \
	MG_ASSERT_MESSAGE((expression) == 0, #expression)

#if defined(NDEBUG)
#	define MG_DEBUG(...)
#else
#	define MG_DEBUG(...)	printf(__VA_ARGS__)
#endif
