#pragma once

#include "gctk_common.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>

GCTK_API bool GctkStartsWithStr(const char* str, const char* start_with);
GCTK_API bool GctkStartsWithChr(const char* str, char start_with);
#ifndef GctkStartWith
	#define GctkStartWith(__str__, __start_with__) _Generic(__start_with__,\
		char*: GctkStartsWithStr,\
		const char*: GctkStartsWithStr,\
		default: GctkStartsWithChr)(__str__, __start_with__)
#endif

GCTK_API bool GctkEndsWithStr(const char* str, const char* ends_with);
GCTK_API bool GctkEndsWithChr(const char* str, char ends_with);
#ifndef GctkEndsWith
#define GctkEndsWith(__str__, __start_with__) _Generic(__start_with__,\
		char*: GctkEndsWithStr,\
		const char*: GctkEndsWithStr,\
		default: GctkEndsWithChr)(__str__, __start_with__)
#endif

GCTK_API errno_t GctkStrCat(char* dest, const char* src, size_t dest_max);
GCTK_API bool GctkStrEq(const char* a, const char* b, bool case_sensitive);

GCTK_API hash_t GctkStrHash(const char* str);

#ifdef __cplusplus
}
#endif