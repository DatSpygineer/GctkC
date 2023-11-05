#pragma once

#include "gctk_common.h"
#include "gctk_string.h"


#ifdef _WIN32
	#define GCTK_PATH_SEP '\\'
	#define GCTK_PATH_SEP_STR "\\"

	#define _CRT_INTERNAL_NONSTDC_NAMES 1
	#include <sys/stat.h>
	#if !defined(S_ISREG) && defined(S_IFMT) && defined(S_IFREG)
		#define S_ISREG(m) (((m) & S_IFMT) == S_IFREG)
	#endif
	#if !defined(S_ISDIR) && defined(S_IFMT) && defined(S_IFDIR)
		#define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)
	#endif
#else
	#include <sys/stat.h>

	#define GCTK_PATH_SEP '/'
	#define GCTK_PATH_SEP_STR "/"
#endif

/**
 * @brief Setup paths for the game.
 * @param name Game's name, used for user and temp path.
 * @param author Game author's name, used for user and temp path.
 * @return Return 'false' if failed, otherwise 'true'.
 */
GCTK_API bool GctkSetupPaths(const char* name, const char* author);

/**
 * @brief Join two strings and adds separator between if needed.
 * @param dest Left hand side string. The right hand side string copied inside this buffer.
 * @param src Right hand side string. This string gets copied to the left hand side buffer.
 * @param buffer_max Maximum capacity of 'dest' (left hand side) string buffer.
 * @return Return '0' on success, otherwise returns an error code. (See: errno)
 */
GCTK_API errno_t GctkJoinPaths(char* dest, const char* src, size_t buffer_max);
/**
 * @brief Check if file at the specific path exists.
 * @param path File path to check.
 */
GCTK_API bool GctkFileExists(const char* path);
/**
 * @brief Check if directory at the specific path exists.
 * @param path Directory path to check.
 */
GCTK_API bool GctkDirExists(const char* path);
/**
 * @brief Create a new directory at the specific path.
 * @param path New directory path.
 * @return Return if the path creation was successful.
 */
GCTK_API bool GctkDirCreate(const char* path);

/**
 * @brief Copy a file to a target path.
 * @param from Path of the file to copy.
 * @param to Path of the destination.
 * @param allow_overwrite Can the destination file overwritten if already existing?
 * @return Return if copying was successful.
 */
GCTK_API bool GctkCopyFile(const char* from, const char* to, bool allow_overwrite);
/**
 * @brief Move a file to a target path.
 * @param from Path of the file to move.
 * @param to Path of the destination.
 * @return Return if moving was successful.
 */
GCTK_API bool GctkMoveFile(const char* from, const char* to);

/**
 * @brief Get user directory path.
 */
GCTK_API const char* GctkUserDir();
/**
 * @brief Get temporary directory path.
 */
GCTK_API const char* GctkTempDir();