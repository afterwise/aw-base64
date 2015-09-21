
#include "aw-base64.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void *encode(size_t *enclen, const void *data, size_t datasize) {
	size_t n = base64len(datasize);
	char *p = malloc(n + 1);
	size_t r = base64(p, n, data, datasize);
	assert(r == n);
	p[n] = 0;
	printf("encoded: %zu->%zu bytes\n", datasize, n);
	printf("%s\n", p);
	return *enclen = n, p;
}

static void *decode(size_t *declen, const void *enc, size_t enclen) {
	int state = 0;
	size_t len = enclen;
	ssize_t r = unbase64(NULL, 0, enc, &len, &state);
	assert(len == 0);
	void *p = malloc(r);
	state = 0;
	len = enclen;
	r = unbase64(p, r, enc, &len, &state);
	assert(len == 0);
	printf("decoded: %zu->%zu bytes\n", enclen, r);
	return *declen = r, p;
}

int main(int argc, char *argv[]) {
	unsigned char *data;
	unsigned char *dec;
	char *enc;
	size_t enclen, declen;
	int i;

	(void) argc;
	(void) argv;

	data = malloc(256);

	for (i = 0; i < 256; ++i)
		data[i] = i;

	enc = encode(&enclen, data, 256);
	dec = decode(&declen, enc, enclen);

	assert(declen == 256);
	assert(memcmp(data, dec, 256) == 0);

	printf("OK\n");
	return 0;
}

