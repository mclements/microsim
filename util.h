#ifndef UTIL_H
#define UTIL_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UTIL_NEW_N(p, n) \
	p = malloc((n) * sizeof *(p)); \
	if (p == NULL) { \
		fprintf(stderr, "malloc failed\n"); \
		exit(EXIT_FAILURE); \
	}

#define UTIL_NEW(p) UTIL_NEW_N(p, 1)

#define UTIL_LEN(a) ((sizeof (a) / sizeof *(a))

/*assigns string or char array src to char array dst*/
#define UTIL_STRSET(dst, src) \
	assert(sizeof (dst) >= sizeof (src)); \
	strcpy((dst), (src));

#endif
