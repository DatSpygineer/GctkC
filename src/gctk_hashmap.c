#include "gctk_mem.h"
#include "gctk_string.h"
#include "gctk_hashmap.h"

static inline int64_t GctkHashmapFindByHash(const HashMap* hashmap, hash_t hash) {
	for (int64_t i = 0; i < hashmap->count; i++) {
		if (hashmap->entries[i].hash == hash) {
			return i;
		}
	}
	return GCTK_NOT_FOUND;
}

void GctkHashmapGenericDeallocator(HashMapEntry* entry) {
	if (entry && entry->data) {
		GctkFree(&entry->data);
	}
}

bool GctkCreateHashmap(HashMap* hashmap, size_t capacity) {
	if (hashmap == NULL) {
		return false;
	}

	HashMapEntry* data = capacity > 0 ? GctkAllocType(HashMapEntry, capacity) : NULL;
	if (data == NULL && capacity > 0) {
		return false;
	}

	*hashmap = (HashMap) {
		.entries = data,
		.capacity = capacity,
		.count = 0
	};
	return true;
}
void GctkFreeHashmap(HashMap* hashmap) {
	if (hashmap && hashmap->entries) {
		for (size_t i = 0; i < hashmap->count; i++) {
			HashMapEntry* entry = &hashmap->entries[i];
			if (entry->dealloc) {
				entry->dealloc(entry);
			}
		}
		free(hashmap->entries);
	}
}

bool GctkHashmapReserve(HashMap* hashmap, size_t capacity) {
	if (hashmap == NULL) {
		return false;
	}

	if (capacity < hashmap->count || hashmap->capacity > capacity) {
		return false;
	}

	if (hashmap->capacity != capacity) {
		GctkResizeType(HashMapEntry, &hashmap->entries, hashmap->capacity, capacity);
		hashmap->capacity = capacity;
	}

	return true;
}
bool GctkHashmapResize(HashMap* hashmap, size_t capacity) {
	if (hashmap == NULL) {
		return false;
	}

	if (capacity < hashmap->count) {
		capacity = hashmap->count;
	}

	if (hashmap->capacity != capacity) {
		GctkResizeType(HashMapEntry, &hashmap->entries, hashmap->capacity, capacity);
		hashmap->capacity = capacity;
	}

	return true;
}
bool GctkHashmapShrink(HashMap* hashmap) {
	if (hashmap == NULL) {
		return false;
	}

	if (hashmap->capacity != hashmap->count) {
		GctkResizeType(HashMapEntry, &hashmap->entries, hashmap->capacity, hashmap->count);
		hashmap->capacity = hashmap->count;
	}

	return true;
}

bool GctkHashmapAdd(HashMap* hashmap, const char* name, const void* data, size_t data_size, HashMapDeallocator deallocator) {
	if (hashmap == NULL) {
		return false;
	}

	hash_t hash = GctkStrHash(name);
	if (GctkHashmapFindByHash(hashmap, hash) >= 0) {
		return false;
	}

	void* p = GctkAlloc(data_size, 1);
	if (p == NULL) {
		return false;
	}
	GctkMemCpy(p, 0, data, 0, 1, data_size);

	if (hashmap->capacity < hashmap->count + 1) {
		GctkHashmapReserve(hashmap, hashmap->count + 1);
	}

	hashmap->entries[hashmap->count++] = (HashMapEntry){
		.hash = hash,
		.data = p,
		.data_size = data_size,
		.dealloc = deallocator
	};

	return true;
}
bool GctkHashmapRemove(HashMap* hashmap, const char* name) {
	int64_t idx = GctkHashmapFind(hashmap, name);
	if (idx < 0) {
		return false;
	}

	HashMapEntry* entry = &hashmap->entries[idx];
	if (entry && entry->dealloc) {
		entry->dealloc(entry);
	}

	GctkMemMove(hashmap->entries, idx, hashmap->entries, idx + 1, hashmap->count - (idx + 1), sizeof(HashMapEntry));
	hashmap->count--;

	return true;
}
HashMapEntry* GctkHashmapGet(HashMap* hashmap, const char* name) {
	int64_t idx = GctkHashmapFind(hashmap, name);
	if (idx < 0) {
		return NULL;
	}
	return &hashmap->entries[idx];
}
const HashMapEntry* GctkHashmapGetConst(const HashMap* hashmap, const char* name) {
	int64_t idx = GctkHashmapFind(hashmap, name);
	if (idx < 0) {
		return NULL;
	}
	return &hashmap->entries[idx];
}
int64_t GctkHashmapFind(const HashMap* hashmap, const char* name) {
	return GctkHashmapFindByHash(hashmap, GctkStrHash(name));
}
bool GctkHashmapContains(const HashMap* hashmap, const char* name) {
	return GctkHashmapFindByHash(hashmap, GctkStrHash(name)) >= 0;
}

inline bool GctkHashmapAddInt(HashMap* hashmap, const char* name, int value) {
	return GctkHashmapAdd(hashmap, name, &value, sizeof(value), NULL);
}

inline bool GctkHashmapAddInt8(HashMap* hashmap, const char* name, int8_t value) {
	return GctkHashmapAdd(hashmap, name, &value, sizeof(value), NULL);
}
inline bool GctkHashmapAddInt16(HashMap* hashmap, const char* name, int16_t value) {
	return GctkHashmapAdd(hashmap, name, &value, sizeof(value), NULL);
}
inline bool GctkHashmapAddInt32(HashMap* hashmap, const char* name, int32_t value) {
	return GctkHashmapAdd(hashmap, name, &value, sizeof(value), NULL);
}
inline bool GctkHashmapAddInt64(HashMap* hashmap, const char* name, int64_t value) {
	return GctkHashmapAdd(hashmap, name, &value, sizeof(value), NULL);
}

inline bool GctkHashmapAddUInt8(HashMap* hashmap, const char* name, uint8_t value) {
	return GctkHashmapAdd(hashmap, name, &value, sizeof(value), NULL);
}
inline bool GctkHashmapAddUInt16(HashMap* hashmap, const char* name, uint16_t value) {
	return GctkHashmapAdd(hashmap, name, &value, sizeof(value), NULL);
}
inline bool GctkHashmapAddUInt32(HashMap* hashmap, const char* name, uint32_t value) {
	return GctkHashmapAdd(hashmap, name, &value, sizeof(value), NULL);
}
inline bool GctkHashmapAddUInt64(HashMap* hashmap, const char* name, uint64_t value) {
	return GctkHashmapAdd(hashmap, name, &value, sizeof(value), NULL);
}

inline bool GctkHashmapAddFloat(HashMap* hashmap, const char* name, float value) {
	return GctkHashmapAdd(hashmap, name, &value, sizeof(value), NULL);
}
inline bool GctkHashmapAddDouble(HashMap* hashmap, const char* name, double value) {
	return GctkHashmapAdd(hashmap, name, &value, sizeof(value), NULL);
}

inline bool GctkHashmapAddString(HashMap* hashmap, const char* name, const char* value) {
	const size_t str_capacity = strlen(value) + 1;
	char* p = GctkAllocType(char, str_capacity);
	memset(p, 0, str_capacity);
	strcpy(p, value);
	return GctkHashmapAdd(hashmap, name, p, str_capacity - 1, &GctkHashmapGenericDeallocator);
}