#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <magica/uint256.h>
#include <magica/decimal_private_def.h>
#include <magica/decimal_private_impl.h>
#include <magica/decimal.h>

MG_DECIMAL_API int mg_decimal_compare(const mg_decimal *op1, const mg_decimal *op2)
{
	int sign1, sign2;
	int scale1, scale2;
	int status1, status2;
	mg_uint256 buf1, buf2, buf3;
	mg_uint256 *fraction1 = &buf1, *fraction2 = &buf2, *tmp = &buf3;

	__mg_decimal_parse(
		op1, /*out*/&sign1, /*out*/&scale1, /*out*/fraction1, /*out*/&status1);
	__mg_decimal_parse(
		op2, /*out*/&sign2, /*out*/&scale2, /*out*/fraction2, /*out*/&status2);

	if(status1 == DECIMAL_STATUS_NAN || status2 == DECIMAL_STATUS_NAN) {
		return 0;
	}
	if(status1 == DECIMAL_STATUS_ZERO || status2 == DECIMAL_STATUS_ZERO) {
		if(status1 != DECIMAL_STATUS_ZERO) {
			return sign1 == SIGN_POSITIVE ? 1: -1;
		} else if(status2 != DECIMAL_STATUS_ZERO) {
			return sign2 == SIGN_POSITIVE ? -1: 1;
		} else {
			return 0;
		}
	}

	if(sign1 != sign2) {
		if(sign1 == SIGN_POSITIVE)
			return 1;
		else 
			return -1;
	}

	if(status1 == DECIMAL_STATUS_INF || status2 == DECIMAL_STATUS_INF) {
		if(status1 == DECIMAL_STATUS_INF || status2 != DECIMAL_STATUS_INF) {
			return 1;
		} else if(status1 != DECIMAL_STATUS_INF || status2 == DECIMAL_STATUS_INF) {
			return -1;
		} else {
			return 0;
		}
	}

	if(scale1 == scale2)
		return mg_uint256_compare(fraction1, fraction2);
	else if(scale1 + DIGIT_MAX < scale2) {
		return -1;
	} else if(scale1 < scale2) {
		mg_uint256_mul128(fraction2, mg_uint256_get_10eN(scale2 - scale1), tmp);
		return mg_uint256_compare(fraction1, tmp);
	} else if(scale1 - DIGIT_MAX > scale2) {
		return 1;
	} else {
		mg_uint256_mul128(fraction1, mg_uint256_get_10eN(scale1 - scale2), tmp);
		return mg_uint256_compare(tmp, fraction2);
	}
}
