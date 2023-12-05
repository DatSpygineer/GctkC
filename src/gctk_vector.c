#include "gctk_vector.h"

#include "gctk_mem.h"

bool GctkCreateVector(Vector* vector, size_t capacity, size_t item_size, VectorItemDeallocator deallocator) {
	if (!vector) {
		return false;
	}

	vector->data = capacity > 0 ? GctkAlloc(item_size, capacity) : NULL;
	vector->item_size = item_size;
	vector->capacity = capacity;
	vector->deallocator = deallocator;
	vector->count = 0;

	return true;
}
bool GctkVectorResize(Vector* vector, size_t capacity, bool allow_delete) {
	if (!vector) {
		return false;
	}

	if (!allow_delete && vector->count > capacity) {
		capacity = vector->count;
	}

	if (vector->capacity == capacity) return false;

	size_t target_size = vector->item_size * capacity;
	if (vector->data == NULL) {
		vector->data = malloc(target_size);
		if (vector->data == NULL) return false;
	} else {
		if (capacity < vector->count) {
			GctkVectorPopN(vector, vector->count - capacity);
		}

		GctkResize(&vector->data, vector->capacity, capacity, vector->item_size);
		if (vector->data == NULL) return false;
	}

	vector->capacity = capacity;
	return true;
}
bool GctkVectorInsert(Vector* vector, size_t idx, const void* data) {
	if (vector == NULL) {
		return false;
	}

	if (idx > vector->count) {
		return GctkVectorPush(vector, data);
	}

	if (vector->count + 1 > vector->capacity) {
		if (!GctkVectorResize(vector, vector->capacity + 1, false)) {
			return false;
		}
	}

	GctkMemMove(vector->data, idx, vector->data, idx + 1, vector->count - (idx + 1), vector->item_size);
	GctkMemCpy(GctkVectorData(vector, idx), 0, data, 0, 1, vector->item_size);

	return true;
}
bool GctkVectorPush(Vector* vector, const void* data) {
	if (vector == NULL) {
		return false;
	}

	if (vector->count + 1 > vector->capacity) {
		if (!GctkVectorResize(vector, vector->capacity + 1, false)) {
			return false;
		}
	}

	GctkMemCpy(vector->data + (vector->count * vector->item_size), 0, data, 0, 1, vector->item_size);
	vector->count++;
	return true;
}
inline bool GctkVectorPop(Vector* vector) {
	if (vector == NULL && vector->count > 0) {
		return false;
	}

	void* p = &vector->data + ((vector->count - 1) * vector->item_size);
	if (vector->deallocator != NULL) {
		vector->deallocator(p, vector->item_size);
	}
	memset(p, 0, vector->item_size);
	vector->count--;

	return true;
}
inline bool GctkVectorPopN(Vector* vector, size_t n) {
	if (vector == NULL) {
		return false;
	}

	for (size_t i = 0; i < n; i++) {
		if (!GctkVectorPop(vector)) {
			return false;
		}
	}
	return true;
}
bool GctkVectorRemove(Vector* vector, size_t idx) {
	if (vector != NULL && vector->count > 0) {
		void* p = GctkVectorData(vector, idx);
		if (vector->deallocator) {
			vector->deallocator(p, vector->item_size);
		}
		GctkMemMove(vector->data, idx, vector->data, idx + 1, vector->count - (idx + 1), vector->item_size);
		vector->count--;
		return true;
	}
	return false;
}
inline void* GctkVectorData(Vector* vector, size_t idx) {
	return (vector != NULL && vector->count > 0 && idx < vector->count) ? (vector->data + (vector->item_size * idx)) : NULL;
}
inline const void* GctkVectorDataConst(const Vector* vector, size_t idx) {
	return (vector != NULL && vector->count > 0 && idx < vector->count) ? (vector->data + (vector->item_size * idx)) : NULL;
}
ssize_t GctkVectorFind(const Vector* vector, const void* data_to_find) {
	if (vector != NULL && data_to_find != NULL) {
		for (ssize_t i = 0; i < vector->count; i++) {
			if (memcmp(vector->data + i * vector->item_size, data_to_find, vector->item_size) == 0) {
				return i;
			}
		}
	}
	return -1;
}
bool GctkVectorClear(Vector* vector) {
	if (vector == NULL) {
		return false;
	}

	if (vector->deallocator != NULL) {
		for (size_t i = 0; i < vector->count; i++) {
			vector->deallocator(vector->data + i * vector->item_size, vector->item_size);
		}
	}
	memset(vector->data, 0, vector->capacity * vector->item_size);
	vector->count = 0;

	return true;
}
bool GctkFreeVector(Vector* vector) {
	if (vector && vector->data) {
		vector->capacity = 0;
		GctkVectorClear(vector);
		free(vector->data);
		return true;
	}
	return false;
}

inline bool GctkVectorPushInt(Vector* vector, int value) {
	return vector != NULL && vector->item_size == sizeof(value) && GctkVectorPush(vector, &value);
}

inline bool GctkVectorPushInt8(Vector* vector, int8_t value) {
	return vector != NULL && vector->item_size == sizeof(value) && GctkVectorPush(vector, &value);
}
inline bool GctkVectorPushInt16(Vector* vector, int16_t value) {
	return vector != NULL && vector->item_size == sizeof(value) && GctkVectorPush(vector, &value);
}
inline bool GctkVectorPushInt32(Vector* vector, int32_t value) {
	return vector != NULL && vector->item_size == sizeof(value) && GctkVectorPush(vector, &value);
}
inline bool GctkVectorPushInt64(Vector* vector, int64_t value) {
	return vector != NULL && vector->item_size == sizeof(value) && GctkVectorPush(vector, &value);
}
inline bool GctkVectorPushUInt8(Vector* vector, uint8_t value) {
	return vector != NULL && vector->item_size == sizeof(value) && GctkVectorPush(vector, &value);
}
inline bool GctkVectorPushUInt16(Vector* vector, uint16_t value) {
	return vector != NULL && vector->item_size == sizeof(value) && GctkVectorPush(vector, &value);
}
inline bool GctkVectorPushUInt32(Vector* vector, uint32_t value) {
	return vector != NULL && vector->item_size == sizeof(value) && GctkVectorPush(vector, &value);
}
inline bool GctkVectorPushUInt64(Vector* vector, uint64_t value) {
	return vector != NULL && vector->item_size == sizeof(value) && GctkVectorPush(vector, &value);
}
inline bool GctkVectorPushFloat(Vector* vector, float value) {
	return vector != NULL && vector->item_size == sizeof(value) && GctkVectorPush(vector, &value);
}
inline bool GctkVectorPushDouble(Vector* vector, double value) {
	return vector != NULL && vector->item_size == sizeof(value) && GctkVectorPush(vector, &value);
}