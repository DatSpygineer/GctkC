#pragma once

#include "gctk_common.h"
#include "gctk_math.h"

#ifndef GCTK_NOT_FOUND
	#define GCTK_NOT_FOUND -1
#endif

typedef struct HashMapEntry HashMapEntry;
typedef void (*HashMapDeallocator)(HashMapEntry* entry);

struct HashMapEntry {
	uint64_t hash;
	void* data;
	size_t data_size;
	HashMapDeallocator dealloc;
};

typedef struct {
	HashMapEntry* entries;
	size_t count, capacity;
} HashMap;

GCTK_API void GctkHashmapGenericDeallocator(HashMapEntry* entry);

GCTK_API bool GctkCreateHashmap(HashMap* hashmap, size_t capacity);
GCTK_API void GctkFreeHashmap(HashMap* hashmap);

GCTK_API bool GctkHashmapReserve(HashMap* hashmap, size_t capacity);
GCTK_API bool GctkHashmapResize(HashMap* hashmap, size_t capacity);
GCTK_API bool GctkHashmapShrink(HashMap* hashmap);
GCTK_API bool GctkHashmapAdd(HashMap* hashmap, const char* name, const void* data, size_t data_size, HashMapDeallocator deallocator);
GCTK_API bool GctkHashmapRemove(HashMap* hashmap, const char* name);
GCTK_API HashMapEntry* GctkHashmapGet(HashMap* hashmap, const char* name);
GCTK_API const HashMapEntry* GctkHashmapGetConst(const HashMap* hashmap, const char* name);
GCTK_API int64_t GctkHashmapFind(const HashMap* hashmap, const char* name);
GCTK_API bool GctkHashmapContains(const HashMap* hashmap, const char* name);

// Generic add functions:

GCTK_API inline bool GctkHashmapAddInt(HashMap* hashmap, const char* name, int value);

GCTK_API inline bool GctkHashmapAddInt8(HashMap* hashmap, const char* name, int8_t value);
GCTK_API inline bool GctkHashmapAddInt16(HashMap* hashmap, const char* name, int16_t value);
GCTK_API inline bool GctkHashmapAddInt32(HashMap* hashmap, const char* name, int32_t value);
GCTK_API inline bool GctkHashmapAddInt64(HashMap* hashmap, const char* name, int64_t value);

GCTK_API inline bool GctkHashmapAddUInt8(HashMap* hashmap, const char* name, uint8_t value);
GCTK_API inline bool GctkHashmapAddUInt16(HashMap* hashmap, const char* name, uint16_t value);
GCTK_API inline bool GctkHashmapAddUInt32(HashMap* hashmap, const char* name, uint32_t value);
GCTK_API inline bool GctkHashmapAddUInt64(HashMap* hashmap, const char* name, uint64_t value);

GCTK_API inline bool GctkHashmapAddFloat(HashMap* hashmap, const char* name, float value);
GCTK_API inline bool GctkHashmapAddDouble(HashMap* hashmap, const char* name, double value);

GCTK_API inline bool GctkHashmapAddString(HashMap* hashmap, const char* name, const char* value);
