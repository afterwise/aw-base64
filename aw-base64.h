/* vim: set ts=4 sw=4 noet : */

/*
   Copyright (c) 2014-2025 Malte Hildingsson, malte (at) afterwi.se

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.
 */

#ifndef AW_BASE64_H
#define AW_BASE64_H

#include <sys/types.h>

#if defined(_HAS_CXX17)
# define _base64_unused [[maybe_unused]]
#elif defined(__GNUC__)
# define _base64_unused __attribute__((unused))
#elif defined(_MSC_VER)
# define _base64_unused
#endif

#if defined(__GNUC__)
# define _base64_alwaysinline __attribute__((always_inline)) inline
#elif defined(_MSC_VER)
# define _base64_alwaysinline __forceinline
#endif

#if defined(__GNUC__)
# define _base64_likely(x) __builtin_expect(!!(x), 1)
# define _base64_unlikely(x) __builtin_expect(!!(x), 0)
#else
# define _base64_likely(x) (x)
# define _base64_unlikely(x) (x)
#endif

#define _base64_sel(x,a,b) ((a) + (((b) - (a)) & ((x) >> (sizeof (x) * 8 - 1))))

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_MSC_VER)
typedef signed __int64 base64_ssize_t;
#else
typedef ssize_t base64_ssize_t;
#endif

_base64_alwaysinline
static size_t base64len(size_t n) { return (n + 2) / 3 * 4; }

_base64_unused
static size_t _base64(
		char *__restrict dst, size_t size, const unsigned char *__restrict src, size_t len,
		const char *__restrict al) {
	size_t res = base64len(len);
	if (size >= res)
		for (size_t i = 0; i < len; ++i) {
			unsigned x = src[i] << 16;
			x |= (++i < len ? src[i] : 0) << 8;
			x |= (++i < len ? src[i] : 0) << 0;
			*dst++ = al[x >> 3 * 6 & 0x3f];
			*dst++ = al[x >> 2 * 6 & 0x3f];
			*dst++ = _base64_sel((base64_ssize_t) (len - 0 - i), al[x >> 1 * 6 & 0x3f], 0x3d);
			*dst++ = _base64_sel((base64_ssize_t) (len - 1 - i), al[x >> 0 * 6 & 0x3f], 0x3d);
		}
	return res;
}

_base64_unused
static size_t base64(char *dst, size_t size, const unsigned char *src, size_t len) {
	const char al[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	return _base64(dst, size, src, len, al);
}

_base64_unused
static size_t base64url(char *dst, size_t size, const unsigned char *src, size_t len) {
	const char al[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";
	return _base64(dst, size, src, len, al);
}

_base64_unused
static const unsigned char _unbase64tab[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x3e, 0x00, 0x3f,
	0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e,
	0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x00, 0x00, 0x00, 0x00, 0x3f,
	0x00, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
	0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/* state must be initialized to zero when starting a new stream */

_base64_unused
static base64_ssize_t unbase64(
		unsigned char *__restrict dst, size_t size, const char *__restrict src,
		size_t *__restrict len, int *__restrict state) {
	size_t res = 0;
	for (; *len != 0 && *src != 0; --*len, ++src) {
		if (_base64_unlikely(*src == 0x3d)) {
			do --*len, ++src; while (*len != 0 && *src == 0x3d);
			break;
		}
		int x = _unbase64tab[(unsigned char) *src];
		*state = ((*state << 6 | x) & 0xfff) | ((*state + 0x6000) & ~0xfff);
		if (_base64_unlikely(*state >= 0x8000)) {
			if (*state -= 0x8000, res < size)
				dst[res] = (unsigned char) ((*state & 0xfff) >> (*state >> 12));
			++res;
		}
	}
	return res;
}

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* AW_BASE64_H */

