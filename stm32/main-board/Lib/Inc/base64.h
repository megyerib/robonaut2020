/*
 * Base64 encoding/decoding (RFC1341)
 * Copyright (c) 2005, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

// Adatped to microcontroller by Balazs Megyeri 2019
// Original: https://web.mit.edu/freebsd/head/contrib/wpa/src/utils/base64.c

#ifndef BASE64_H
#define BASE64_H

#include <stddef.h>
#include <stdint.h>

void base64_encode(const uint8_t *src, uint8_t* dst, size_t len, size_t *out_len);
void base64_decode(const uint8_t *src, uint8_t* dst, size_t len, size_t *out_len);

#endif /* BASE64_H */
// TODO reset file type to c and link from the outside
