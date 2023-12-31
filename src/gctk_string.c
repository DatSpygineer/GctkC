#include "gctk_string.h"
#include "crc64.h"

bool GctkStartsWithStr(const char* str, const char* start_with) {
	return strncmp(str, start_with, strlen(start_with)) == 0;
}
bool GctkStartsWithChr(const char* str, char start_with) {
	return str[0] == start_with;
}

bool GctkEndsWithStr(const char* str, const char* ends_with) {
	const size_t end_len = strlen(ends_with);
	const size_t str_end = strlen(str) - 1 - end_len;
	return strncmp(str + str_end, ends_with, end_len) == 0;
}
bool GctkEndsWithChr(const char* str, char ends_with) {
	return str[strlen(str) - 1] == ends_with;
}

errno_t GctkStrCat(char* dest, const char* src, size_t dest_max) {
#ifdef _WIN32
	return strcat_s(dest, dest_max, src);
#else
	strcat(dest, src);
	return errno;
#endif
}

bool GctkStrEq(const char* a, const char* b, bool case_sensitive) {
	if (case_sensitive) {
		return strcmp(a, b) == 0;
	} else {
#ifdef _WIN32
		return _stricmp(a, b) == 0;
#else
		return strcasecmp(a, b) == 0;
#endif
	}
}

hash_t GctkStrHash(const char* str) {
	return crc64(UINT64_MAX, (const uint8_t*)str, strlen(str));
}
