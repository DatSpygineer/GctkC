#include "gctk_mem.h"

#include "gctk_debug.h"

Allocator GctkAllocator = {
	.Alloc = &GctkDefaultAlloc,
	.Resize = &GctkDefaultResize,
	.Free = &GctkDefaultFree
};


void GctkLogMalloc(void* ptr, size_t size, size_t count) {
	if (ptr != NULL) {
		GCTK_LOG(
			"Allocated memory '%p' with size " GCTK_SIZE_FORMAT " and count of " GCTK_SIZE_FORMAT " (overall " GCTK_SIZE_FORMAT ") ",
			ptr, size, count, size * count
		);
	} else {
		GCTK_LOG_FATAL_ERR(GCTK_ERROR_OUT_OF_MEMORY,
			"Failed to allocate memory (size " GCTK_SIZE_FORMAT ", count " GCTK_SIZE_FORMAT ", overall " GCTK_SIZE_FORMAT "): Out of memory",
			size, count, size * count
		);
	}
}
void GctkLogResize(void* ptr_old, void* ptr_new, size_t old_count, size_t new_count, size_t size) {
	if (ptr_new != NULL) {
		GCTK_LOG(
			"Resized block '%p' to '%p' (old count " GCTK_SIZE_FORMAT " -> new count " GCTK_SIZE_FORMAT ", size " GCTK_SIZE_FORMAT ")",
			ptr_old, ptr_new,
			old_count, new_count, size
		);
	} else {
		GCTK_LOG_FATAL_ERR(GCTK_ERROR_OUT_OF_MEMORY,
			"Failed to allocate new block for resizing '%p' (old count \" GCTK_SIZE_FORMAT \" -> new count \" GCTK_SIZE_FORMAT \", size \" GCTK_SIZE_FORMAT \")",
			ptr_old,
			old_count, new_count, size
		);
	}
}
void GctkLogFree(void* ptr) {
	if (ptr != NULL) {
		GCTK_LOG("Freeing memory block %p", ptr);
	}
}

void* GctkDefaultAlloc(size_t size, size_t count) {
	void* ptr = calloc(count, size);
#if defined(GCTK_DEBUG_MALLOC) && (GCTK_DEBUG_MALLOC == 1)
	GctkLogMalloc(ptr, size, count);
#endif
	return ptr;
}
void* GctkDefaultResize(void** ptr, size_t old_count, size_t new_count, size_t size) {
	if (ptr != NULL) {
		if (old_count == new_count) {
			return *ptr;
		}

		void* old = *ptr;
		void* temp = calloc(new_count, size);
		GctkMemCpy(temp, 0, old, 0, old_count > new_count ? new_count : old_count, size);
#if defined(GCTK_DEBUG_MALLOC) && (GCTK_DEBUG_MALLOC == 1)
		GctkLogResize(old, temp, old_count, new_count, size);
#endif
		free(old);
		*ptr = temp;
		return *ptr;
	}
	return NULL;
}
void GctkDefaultFree(void** ptr) {
	if (ptr != NULL) {
#if defined(GCTK_DEBUG_MALLOC) && (GCTK_DEBUG_MALLOC == 1)
		GctkLogFree(ptr);
#endif
		free(*ptr);
		*ptr = NULL;
	}
}

inline void* GctkMemCpy(void* dest, size_t dest_offset, const void* src, size_t src_offset, size_t count, size_t item_size) {
	return memcpy(dest + (dest_offset * item_size), src + (src_offset * item_size), count * item_size);
}
inline void* GctkMemMove(void* dest, size_t dest_offset, const void* src, size_t src_offset, size_t count, size_t item_size) {
	return memmove(dest + (dest_offset * item_size), src + (src_offset * item_size), count * item_size);
}