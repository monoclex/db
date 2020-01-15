#ifndef UNIX_DB_STRCONCAT_C
#define UNIX_DB_STRCONCAT_C
#include <string.h>
#include <stdlib.h> // free

// TODO: make cleaner
static char* str_concat(const char* const a, const char* const b) {
	FUNCTION_IN("str_concat");
	NULL_CHECK(a, "a");
	NULL_CHECK(b, "b");
	
	size_t a_len = strlen(a);
	size_t b_len = strlen(b);

	// +1 for null terminator
	char* result = malloc(a_len + b_len + 1);
	NULL_CHECK(result, "result");
	
#ifdef _WIN32
	for (int i = 0; i < a_len; i++) {
		result[i] = a[i];
	}
	for (int i = 0; i < b_len; i++) {
		result[a_len + i] = b[i];
	}
	// causing issues lmao
	// strcpy_s(result, a_len, a);
	// strcpy_s(result + a_len, b_len, b);
#else
	strcpy(result, a);
	strcpy(result + a_len, b);
#endif

	FUNCTION_OUT("str_concat");
	return result;
}

// explicity free stuff allocated from str_concat
#define CONCATFREE(a) free((a))

#define CONCAT2(a, b) str_concat((a), (b))

#define CONCAT3(a, b, c) str_concat3((a), (b), (c))
static char* str_concat3(const char* const a, const char* const b, const char* const c) {
	
		char* concat_b_c = CONCAT2(b, c);
	char* concat_all = CONCAT2(a, concat_b_c);
		CONCATFREE(concat_b_c);
	
	return concat_all;
}

#define CONCAT5(a, b, c, d, e) str_concat5((a), (b), (c), (d), (e))
static char* str_concat5(const char* const a, const char* const b, const char* const c, const char* const d, const char* const e) {

		char* concat_a_b = CONCAT2(a, b);
		char* concat_c_d_e = CONCAT3(c, d, e);
	char* concat_all = CONCAT2(concat_a_b, concat_c_d_e);
		CONCATFREE(concat_a_b);
		CONCATFREE(concat_c_d_e);

	return concat_all;
}

#endif
