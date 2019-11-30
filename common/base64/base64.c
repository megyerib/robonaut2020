/*
 * Base64 encoding/decoding (RFC1341)
 * Copyright (c) 2005-2011, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#include "base64.h"

#include <stdlib.h>
#include <string.h>

static const unsigned char base64_table[65] =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// TODO decode table for faster decoding

/**
 * base64_encode - Base64 encode
 * @src: Data to be encoded
 * @dst: Destination buffer
 * @len: Length of the data to be encoded
 * @out_len: Pointer to output length variable, or %NULL if not used
 *
 * Returned buffer is nul terminated to make it easier to use as a C string.
 * The nul terminator is not included in out_len.
 */
void base64_encode(const uint8_t *src, uint8_t* dst, size_t len, size_t *out_len)
{
    unsigned char *pos;
	const unsigned char *end, *in;

	end = src + len;
	in = src;
    pos = dst;

    while (end - in >= 3)
    {
		*pos++ = base64_table[in[0] >> 2];
		*pos++ = base64_table[((in[0] & 0x03) << 4) | (in[1] >> 4)];
		*pos++ = base64_table[((in[1] & 0x0f) << 2) | (in[2] >> 6)];
		*pos++ = base64_table[in[2] & 0x3f];
        in += 3;
	}

    if (end - in)
    {
		*pos++ = base64_table[in[0] >> 2];
        if (end - in == 1)
        {
			*pos++ = base64_table[(in[0] & 0x03) << 4];
            //*pos++ = '=';
        }
        else
        {
			*pos++ = base64_table[((in[0] & 0x03) << 4) |
					      (in[1] >> 4)];
			*pos++ = base64_table[(in[1] & 0x0f) << 2];
		}
        //*pos++ = '=';
    }

	*pos = '\0';
	if (out_len)
    {
        *out_len = (size_t)(pos - dst);
    }
}


/**
 * base64_decode - Base64 decode
 * @src: Data to be decoded
 * @len: Length of the data to be decoded
 * @out_len: Pointer to output length variable
 * Returns: Allocated buffer of out_len bytes of decoded data,
 * or %NULL on failure
 *
 * Caller is responsible for freeing the returned buffer.
 */
void base64_decode(const uint8_t *src, uint8_t* dst, size_t len, size_t *out_len)
{
    unsigned char dtable[256], *pos, block[4], tmp;
    size_t i, count;

    memset(dtable, 0x80, 256);
	for (i = 0; i < sizeof(base64_table) - 1; i++)
    {
		dtable[base64_table[i]] = (unsigned char) i;
    }

    pos = dst;

	count = 0;
    for (i = 0; i < len; i++)
    {
		tmp = dtable[src[i]];

        if (tmp != 0x80)
        {
            block[count] = tmp;
            count++;

            if (count == 4)
            {
                *pos++ = (uint8_t)((block[0] << 2) | (block[1] >> 4));
                *pos++ = (uint8_t)((block[1] << 4) | (block[2] >> 2));
                *pos++ = (uint8_t)((block[2] << 6) | block[3]);
                count = 0;
            }
        }
	}

    if (count == 1)
    {
        tmp = (uint8_t)(block[0] << 2);

        if (tmp != 0)
        {
            *pos++ = tmp;
        }
    }
    else if (count == 2)
    {
        *pos++ = (uint8_t)((block[0] << 2) | (block[1] >> 4));

        tmp = (uint8_t)(block[1] << 4);

        if (tmp != 0)
        {
            *pos++ = tmp;
        }
    }
    else if (count == 3)
    {
        *pos++ = (uint8_t)((block[0] << 2) | (block[1] >> 4));
        *pos++ = (uint8_t)((block[1] << 4) | (block[2] >> 2));

        tmp = (uint8_t)(block[2] << 6);

        if (tmp != 0)
        {
            *pos++ = tmp;
        }
    }

    *out_len = (size_t)(pos - dst);
}
