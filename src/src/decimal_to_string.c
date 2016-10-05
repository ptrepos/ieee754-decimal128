#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <magica/uint256.h>
#include <magica/decimal_private_def.h>
#include <magica/decimal_private_impl.h>
#include <magica/decimal.h>

typedef struct {
	char *buf;
	int buf_size;
	int index;
} c_buffer;

static void add_c(c_buffer *buf, char c)
{
	if (buf->index < buf->buf_size) {
		buf->buf[buf->index] = c;
	}
	buf->index += 1;
}

static int encode(const mg_uint256 *value, char *buf)
{
	int i = 0;
	mg_uint256 v, r, tmp;

	v = *value;
	while (!mg_uint256_is_zero(&v)) {
		mg_uint256_div(&v, mg_uint256_get_10eN(8), /*out*/&tmp, &r);
		v = tmp;

		if (mg_uint256_is_zero(&v)) {
			uint32_t sub = mg_uint256_get_uint32(&r);
			while (sub != 0) {
				buf[i++] = sub % 10 + '0';
				sub /= 10;
			}
			break;
		} else {
			uint32_t sub = mg_uint256_get_uint32(&r);
			for (int j = 0; j < 8; j++) {
				buf[i++] = sub % 10 + '0';
				sub /= 10;
			}
		}
	}
	
	int max = i;
	for(i = 0; i < max / 2; i++) {
		char c = buf[i];
		buf[i] = buf[max-i-1];
		buf[max-i-1] = c;
	}

	return max;
}

MG_DECIMAL_API mg_decimal_error mg_decimal_to_string(
	const mg_decimal *value,
	char *buf,
	int buf_size,
	int *required_buf_size)
{
	mg_decimal_error err;
	int sign, scale, status;
	mg_uint256 buf1, buf2;
	mg_uint256 *fraction = &buf1, *tmp = &buf2;

	c_buffer obuf;
	obuf.buf = buf;
	obuf.buf_size = buf_size;
	obuf.index = 0;

	__mg_decimal_parse(
		value, /*out*/&sign, /*out*/&scale, /*out*/fraction, /*out*/&status);
	if(status == DECIMAL_STATUS_INF) {
		add_c(&obuf, 'i');
		add_c(&obuf, 'n');
		add_c(&obuf, 'f');
		add_c(&obuf, 0);
		goto _EXIT;
	} else if(status == DECIMAL_STATUS_NAN) {
		add_c(&obuf, 'n');
		add_c(&obuf, 'a');
		add_c(&obuf, 'n');
		add_c(&obuf, 0);
		goto _EXIT;
	} else if(status == DECIMAL_STATUS_ZERO) {
		add_c(&obuf, '0');
		add_c(&obuf, 0);
		goto _EXIT;
	}
	
	char c[DIGIT_MAX*2];
	int digits = encode(fraction, /*out*/c);
	
	if(digits <= 0) {
		add_c(&obuf, '0');
		add_c(&obuf, 0);
	} else {
		if(sign == SIGN_NEGATIVE) {
			add_c(&obuf, '-');
		}

		if(scale >= 0) {
			int i = 0;

			while(i < digits) {
				add_c(&obuf, c[i++]);
			}
			while(scale > 0) {
				add_c(&obuf, '0');
				scale--;
			}
			add_c(&obuf, 0);
		} else if(scale > -digits) {
			int i = 0;

			while(i < digits) {
				add_c(&obuf, c[i++]);
				if(digits + scale == i) {
					add_c(&obuf, '.');
				}
			}
			add_c(&obuf, 0);
		} else {
			int i = 0;

			add_c(&obuf, '0');
			add_c(&obuf, '.');

			while(scale < -digits) {
				add_c(&obuf, '0');
				scale++;
			}
			while(i < digits) {
				add_c(&obuf, c[i++]);
			}
			add_c(&obuf, 0);
		}
	}

_EXIT:
	if(required_buf_size != NULL) {
		*required_buf_size = obuf.index;
	}
	
	if(obuf.index > buf_size) {
		err = MG_DECIMAL_ERROR_BUFFER_NOT_ENOUGH;
		goto _ERROR;
	}

	return 0;
_ERROR:
	return err;
}
