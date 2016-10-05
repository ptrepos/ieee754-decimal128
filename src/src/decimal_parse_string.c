#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <magica/uint256.h>
#include <magica/decimal_private_def.h>
#include <magica/decimal_private_impl.h>
#include <magica/decimal.h>

MG_DECIMAL_API mg_decimal_error mg_decimal_parse_string(const char *value, /*out*/mg_decimal *ret)
{
	mg_decimal_error err;
	int sign;
	int scale;
	int digits;
	int c;
	mg_uint256 buf1, buf2;
	mg_uint256 *fraction = &buf1, *tmp = &buf2;

	if(*value == 0) {
		err = MG_DECIMAL_ERROR_CONVERT;
		goto _ERROR;
	}
	
	// ïÑçÜÇÃèàóù
	switch(*value) {
	case '-':
		sign = SIGN_NEGATIVE;
		value++;
		break;
	case '+':
		value++;
	default:
		sign = SIGN_POSITIVE;
		break;
	}

	scale = 0;
	digits = 0;
	mg_uint256_set_zero(fraction);

	if(*value == 0) {
		err = MG_DECIMAL_ERROR_CONVERT;
		goto _ERROR;
	}

	// êÊì™ÇÃ0Çèàóù
	while(*value != 0) {
		c = *value;
		if(c == '0') {
			value++;
			// increment next
		} else if(('1' <= c && c <= '9') || c == '.') {
			break;
		} else  {
			err = MG_DECIMAL_ERROR_CONVERT;
			goto _ERROR;
		}
	}

	// êÆêîïîÇÃèàóù
	bool has_dot = false;
	bool has_exp = false;
	while(*value != 0) {
		c = *value;
		if('0' <= c && c <= '9') {
			if(digits < DIGIT_MAX) {
				// fraction * 10 + c
				mg_uint256_mul128(fraction, mg_uint256_get_10eN(1), tmp);
				mg_uint256_set(fraction, c - '0');
				mg_uint256_add_1(/*inout*/fraction, tmp);
				digits++;
			} else {
				scale++;
			}
			value++;
		} else if(c == '.') {
			has_dot = true;
			break;
		} else if(c == 'e' || c =='E') {
			has_exp = true;
			break;
		}  else {
			err = MG_DECIMAL_ERROR_CONVERT;
			goto _ERROR;
		}
	}
	
	// è≠êîïîÇÃèàóù
	if(has_dot)
	{
		value++;
		if(*value == 0) {
			err = MG_DECIMAL_ERROR_CONVERT;
			goto _ERROR;
		}

		if(digits <= 0) {
			while(*value != 0) {
				int c = *value;
				if(c != '0') {
					break;
				}
				scale--;
				value++;
			}
		}
		
		while(*value != 0) {
			int c = *value;
			if('0' <= c && c <= '9') {
				if(scale <= 0 && digits < DIGIT_MAX) {
					// fraction * 10 + c
					mg_uint256_mul128(fraction, mg_uint256_get_10eN(1), tmp);
					mg_uint256_set(fraction, c - '0');
					mg_uint256_add_1(/*inout*/fraction, tmp);
					digits++;
					
					scale--;
				}
				value++;
			} else if(c == 'e' || c =='E') {
				has_exp = true;
				break;
			} else {
				err = MG_DECIMAL_ERROR_CONVERT;
				goto _ERROR;
			}
		}
	}
	
	// 10^nïîÇÃèàóù
	if(has_exp) {
		value++;
		if(*value == 0) {
			err = MG_DECIMAL_ERROR_CONVERT;
			goto _ERROR;
		}
		
		int exp_sign;
		switch(*value) {
		case '-':
			exp_sign = SIGN_NEGATIVE;
			value++;
			break;
		case '+':
			value++;
		default:
			exp_sign = SIGN_POSITIVE;
			break;
		}
		
		unsigned int exp = 0;
		while(*value != 0) {
			int c = *value;
			if('0' <= c && c <= '9') {
				exp = exp * 10 + (c - '0');
			} else {
				err = MG_DECIMAL_ERROR_CONVERT;
				goto _ERROR;
			}
			if(c > 1000000000) {
				err = MG_DECIMAL_ERROR_OVERFLOW;
				goto _ERROR;
			}
			value++;
		}
		
		if(exp_sign == SIGN_POSITIVE) {
			scale += exp;
		} else {
			scale -= exp;
		}
	}

	err = __mg_decimal_set_1(ret, sign, scale, fraction);
	if(err != 0)
		goto _ERROR;

	return 0;
_ERROR:
	return err;
}
