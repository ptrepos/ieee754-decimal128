/**
* Copyright (C) Takuo Hada 2015-2016
* @author t.hada 2016/07/09
*
* 256 bit integer for decimal.
* this source includes decimal implementation.
*/
#include <assert.h>
#include <magica/int/mgcompat.h>
#include <magica/int/uint256_def.h>
#include <magica/int/uint256_impl.h>

MG_PRIVATE void mg_uint256_test_to_string(const mg_uint256 *value, char *buf)
{
	int index;
	mg_uint256 v, v10, quotient, reminder;

	mg_uint256_set(&v10, 10);

	v = *value;

	index = 0;
	do {
		mg_uint256_div(&v, &v10, /*out*/&quotient, /*out*/&reminder);

		uint64_t s = mg_uint256_get_uint64(&reminder);
		buf[index++] = (char)(s + '0');

		v = quotient;
	} while(!mg_uint256_is_zero(&v));

	for(int i = 0; i < index / 2; i++) {
		char c = buf[i];
		buf[i] = buf[index - i - 1];
		buf[index - i - 1] = c;
	}
	buf[index] = 0;
}

MG_PRIVATE void mg_uint256_test_convert(const char *buf, mg_uint256 *value)
{
	mg_uint256 v, v10, n, tmp, tmp2;

	mg_uint256_set(&v10, 10);

	mg_uint256_set_zero(&v);

	int i = 0;
	while(buf[i] != 0) {
		int overflow = mg_uint256_mul(&v, &v10, /*out*/&tmp);
		assert(overflow == 0);

		mg_uint256_set(&n, buf[i] - '0');
		mg_uint256_add(&tmp, &n, /*out*/&tmp2);

		v = tmp2;
		i++;
	}
	*value = v;
}

MG_PRIVATE void mg_uint256_test_to_hex_string(const mg_uint256 *value, char *buf)
{
	int index;
	mg_uint256 v, reminder, quotient;
	mg_uint256 v16;

	mg_uint256_set(&v16, 16);

	v = *value;

	index = 0;
	do {
		mg_uint256_div(&v, &v16, &quotient, &reminder);

		uint64_t s = mg_uint256_get_uint64(&reminder);
		buf[index++] = s < 10 ? (char)(s + '0'): (char)(s + 'A' - 10);

		v = quotient;
	} while(!mg_uint256_is_zero(&v));

	for(int i = 0; i < index / 2; i++) {
		char c = buf[i];
		buf[i] = buf[index - i - 1];
		buf[index - i - 1] = c;
	}
	buf[index] = 0;
}

MG_PRIVATE void mg_uint256_test_hex_convert(const char *buf, mg_uint256 *value)
{
	mg_uint256 v, tmp, tmp2, n;
	mg_uint256 v16;

	mg_uint256_set(&v16, 16);

	mg_uint256_set_zero(&v);

	int i = 0;
	while(buf[i] != 0) {
		int overflow = mg_uint256_mul(&v, &v16, /*out*/&tmp);
		assert(overflow == 0);

		mg_uint256_set(&n, buf[i] >= 'A' ? buf[i] - 'A' + 10: buf[i] - '0');
		mg_uint256_add(&tmp, &n, &tmp2);

		v = tmp2;
		i++;
	}
	*value = v;
}
