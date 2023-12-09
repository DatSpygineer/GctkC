#pragma once

#include "gctk_common.h"

typedef void (*VectorItemDeallocator)(void* data, size_t size);

typedef struct {
	void* data;
	size_t count, capacity, item_size;
	VectorItemDeallocator deallocator;
} Vector;

GCTK_API bool GctkCreateVector(Vector* vector, size_t capacity, size_t item_size, VectorItemDeallocator deallocator);
GCTK_API bool GctkVectorResize(Vector* vector, size_t capacity, bool allow_delete);
GCTK_API bool GctkVectorInsert(Vector* vector, size_t idx, const void* data);
GCTK_API bool GctkVectorPush(Vector* vector, const void* data);
GCTK_API bool GctkVectorPop(Vector* vector);
GCTK_API bool GctkVectorPopN(Vector* vector, size_t n);
GCTK_API bool GctkVectorRemove(Vector* vector, size_t idx);
GCTK_API void* GctkVectorData(Vector* vector, size_t idx);
GCTK_API const void* GctkVectorDataConst(const Vector* vector, size_t idx);
GCTK_API ssize_t GctkVectorFind(const Vector* vector, const void* data_to_find);
GCTK_API bool GctkVectorClear(Vector* vector);
GCTK_API bool GctkFreeVector(Vector* vector);

#define GctkVectorAt(T, __pvector__, __idx__) ((T*)GctkVectorData(__pvector__, __idx__))
#define GctkVectorAtConst(T, __pvector__, __idx__) ((const T*)GctkVectorDataConst(__pvector__, __idx__)

// Generic add functions:

GCTK_API bool GctkVectorPushInt(Vector* vector, int value);

GCTK_API bool GctkVectorPushInt8(Vector* vector, int8_t value);
GCTK_API bool GctkVectorPushInt16(Vector* vector, int16_t value);
GCTK_API bool GctkVectorPushInt32(Vector* vector, int32_t value);
GCTK_API bool GctkVectorPushInt64(Vector* vector, int64_t value);

GCTK_API bool GctkVectorPushUInt8(Vector* vector, uint8_t value);
GCTK_API bool GctkVectorPushUInt16(Vector* vector, uint16_t value);
GCTK_API bool GctkVectorPushUInt32(Vector* vector, uint32_t value);
GCTK_API bool GctkVectorPushUInt64(Vector* vector, uint64_t value);

GCTK_API bool GctkVectorPushFloat(Vector* vector, float value);
GCTK_API bool GctkVectorPushDouble(Vector* vector, double value);

#define FOREACH_VECTOR(T, __item__, __pvector__) for (T* __item__ = (T*)(__pvector__)->data; __item__ < (((T*)(__pvector__)->data) + (__pvector__)->count); __item__++)
#define FOREACH_VECTOR_CONST(T, __item__, __pvector__) for (const T* __item__ = (const T*)(__pvector__)->data; __item__ < (((const T*)(__pvector__)->data) + (__pvector__)->count); __item__++)