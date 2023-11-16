#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#ifdef _WIN32
	#include <windows.h>
	#include <ShObjIdl_core.h>
	#define GCTK_PATH_MAX 512 //MAX_PATH
	#define GCTK_SIZE_FORMAT "%llu"
#else
	#include <unistd.h>
	#include <limits.h>
	#include <sys/types.h>
	#include <pwd.h>
	#define GCTK_PATH_MAX 512 //PATH_MAX
	#define GCTK_SIZE_FORMAT "%lu"
	typedef int errno_t;
#endif

#ifdef __cplusplus
}
#endif

#ifdef GCTK_DLL_EXPORT
	#ifdef _WIN32
		#define GCTK_API __declspec(dllexport)
	#else
		#define GCTK_API __attribute__((visibility("default")))
	#endif
	#define GCTK_CONST extern const
	#define GCTK_CONST_IMPL GCTK_API const
#else
	#ifdef _WIN32
		#define GCTK_API __declspec(dllimport)
		#define GCTK_CONST GCTK_API const
		#define GCTK_CONST_IMPL const
	#else
		#define GCTK_API
		#define GCTK_CONST extern const
	#endif
#endif