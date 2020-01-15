#ifndef UNIX_DB_GUID_C
#define UNIX_DB_GUID_C

#include <stdlib.h> // malloc
#include <stdio.h> // sprintf
#include <stdint.h> // uint16_t and friends

typedef struct guid_t {
	int64_t upper;
	int64_t lower;
} guid_t;

static char* alloc_guid_to_string(guid_t guid) {
	char* guid_chars = malloc(37);

	uint32_t section_one =	 (guid.upper & 0xFFFFFFFF00000000) >> 32;
	uint16_t section_two =	 (guid.upper & 0x00000000FFFF0000) >> 16;
	uint16_t section_three = (guid.upper & 0x000000000000FFFF) >> 0;
	uint16_t section_four =	 (guid.lower & 0xFFFF000000000000) >> 48;
	uint64_t section_five =	 (guid.lower & 0x0000FFFFFFFFFFFF) >> 0;

#ifdef _WIN32
	sprintf(guid_chars, "%08X-%04X-%04X-%04X-%012llX", section_one, section_two, section_three, section_four, section_five);
#else
	sprintf(guid_chars, "%08X-%04X-%04X-%04X-%012lX", section_one, section_two, section_three, section_four, section_five);
#endif
	return guid_chars;
}

#endif
