#ifndef UNIX_DB_RANDOM_C
#define UNIX_DB_RANDOM_C
#include <stdint.h> // size_t

#ifdef _WIN32
#define _CRT_RAND_S
#include <stdlib.h> // srand
#include <string.h> // memcpy
#include <stdbool.h> // boolean for random_fill_seeded
#include <time.h> // time
#else
#include <stdlib.h> // TODO: figure out why this is included
#include <stdio.h> // FILE for reading from /dev/urandom
#endif

static void random_fill(void* data, size_t length);

#ifdef _WIN32

// TODO: crypotographically secure
// see https://stackoverflow.com/a/7399380
static bool random_fill_seeded = false;

static void random_fill(void* data, size_t length)
{
	if (!random_fill_seeded) {
		srand(time(NULL));
		random_fill_seeded = true;
	}

	size_t filled = 0;
	uint32_t rng;

	while (filled < length) {
		rand_s(&rng);

		size_t bytes_to_copy = sizeof(rng);
		size_t left = length - filled;

		if (left < bytes_to_copy) {
			bytes_to_copy = left;
		}

		memcpy(data + filled, &rng, bytes_to_copy);
		filled += bytes_to_copy;
	}
}
#else
// already cryptographically secure
static void random_fill(void* data, size_t length)
{
	FILE* file = fopen("/dev/urandom", "r");
	fread(data, sizeof(char), length, file);
	fclose(file);
}
#endif

#endif
