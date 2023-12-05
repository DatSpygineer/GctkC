#pragma once

#include "gctk_common.h"

typedef struct {
	void* (*Alloc)(size_t size, size_t count);
	void* (*Resize)(void** ptr, size_t old_count, size_t new_count, size_t size);
	void (*Free)(void** ptr);
} Allocator;

extern Allocator GctkAllocator;

#define GctkAlloc(__size__, __count__) GctkAllocator.Alloc(__size__, __count__)
#define GctkAllocType(T, __count__) ((T*)GctkAllocator.Alloc(sizeof(T), __count__))
#define GctkResize(__ptr_ptr__, __old_count__, __new_count__, __size__) GctkAllocator.Resize((void**)(__ptr_ptr__), __old_count__, __new_count__, __size__)
#define GctkResizeType(T, __ptr_ptr__, __old_count__, __new_count__) ((T*)GctkAllocator.Resize((void**)(__ptr_ptr__), __old_count__, __new_count__, sizeof(T)))
#define GctkFree(__ptr_ptr__) GctkAllocator.Free(__ptr_ptr__)

GCTK_API void* GctkDefaultAlloc(size_t size, size_t count);
GCTK_API void* GctkDefaultResize(void** ptr, size_t old_count, size_t new_count, size_t size);
GCTK_API void GctkDefaultFree(void** ptr);

GCTK_API void GctkLogMalloc(void* ptr, size_t size, size_t count);
GCTK_API void GctkLogResize(void* ptr_old, void* ptr_new, size_t old_count, size_t new_count, size_t size);
GCTK_API void GctkLogFree(void* ptr);

GCTK_API inline void* GctkMemCpy(void* dest, size_t dest_offset, const void* src, size_t src_offset, size_t count, size_t item_size);
GCTK_API inline void* GctkMemMove(void* dest, size_t dest_offset, const void* src, size_t src_offset, size_t count, size_t item_size);