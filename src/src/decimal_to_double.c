#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>
#include <float.h>
#include <magica/uint256.h>
#include <magica/decimal_private_def.h>
#include <magica/decimal_private_impl.h>
#include <magica/decimal.h>

static double __power(int n)
{
	if(n <= 0) {
		return 1.0;
	} else if(n <= 1) {
		return 10.0;
	}

	double v = __power(n / 2);
	v = v * v;

	int b = n / 2;
	if(v == 0)
		return v;
	else 
		return v * __power(n % 2);
}

MG_DECIMAL_API mg_decimal_error mg_decimal_to_double(const mg_decimal *value, /*out*/double *ret)
{
	mg_decimal_error err;
	int sign, scale, status;
	mg_uint256 fraction, high, low;

	__mg_decimal_parse(
		value, /*out*/&sign, /*out*/&scale, /*out*/&fraction, /*out*/&status);
	if(status == DECIMAL_STATUS_INF) {
		err = MG_DECIMAL_ERROR_OVERFLOW;
		goto _ERROR;
	} else if(status == DECIMAL_STATUS_NAN) {
		err = MG_DECIMAL_ERROR_NAN;
		goto _ERROR;
	} else if(status == DECIMAL_STATUS_ZERO) {
		*ret = 0.0;
		goto _EXIT;
	}

	int ierr = mg_uint256_div(
		&fraction, 
		mg_uint256_get_10eN(17), 
		/*out*/&high, 
		/*out*/&low);
	assert(ierr == 0);

	double dlow = (double)mg_uint256_get_uint64(&low);
	double dhigh = (double)mg_uint256_get_uint64(&high);
	double dfraction = (dhigh * 1e+17 + dlow);
	
	double abs;
	if(scale < 0) {
		abs = dfraction / __power(-scale);
	} else {
		abs = dfraction * __power(scale);
	}

	if(sign == SIGN_NEGATIVE) {
		*ret = -abs;
	} else {
		*ret = abs;
	}

	if(isnan(*ret)) {
		err = MG_DECIMAL_ERROR_NAN;
		goto _ERROR;
	} else if(isinf(*ret)) {
		err = MG_DECIMAL_ERROR_OVERFLOW;
		goto _ERROR;
	}

_EXIT:
	return 0;
_ERROR:
	return err;
}
