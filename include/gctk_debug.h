#pragma once

#include <stdarg.h>
#include <time.h>

#include "gctk_common.h"
#include "gctk_filesys.h"

typedef enum ErrorCode {
	// No error present
	GCTK_NO_ERROR,
	// Failed to initialize GLFW
	GCTK_ERROR_GLFW_INIT,
	// Failed to initialize GLEW
	GCTK_ERROR_GLEW_INIT,
	// Failed to create window
	GCTK_ERROR_WINDOW_INIT,
	// Shader compile error
	GCTK_ERROR_SHADER_COMPILE,
	// Shader link error
	GCTK_ERROR_SHADER_LINK,
	// Failed to load texture
	GCTK_ERROR_LOAD_TEXTURE,
	// OpenGL runtime error
	GCTK_ERROR_OPENGL_RUNTIME,
	// Failed to initialize Lua
	GCTK_ERROR_LUA_INIT,
	// Lua runtime error
	GCTK_ERROR_LUA_RUNTIME,
	// Index out of range error
	GCTK_ERROR_OUT_OF_RANGE,
	// Failed to allocate memory
	GCTK_ERROR_OUT_OF_MEMORY,
	// Unexpected null pointer
	GCTK_ERROR_NULLPTR,
	// Failed to open file.
	GCTK_ERROR_FILE_OPEN,
	// Failed to read file
	GCTK_ERROR_FILE_READ,
	// Failed to write file
	GCTK_ERROR_FILE_WRITE,
	// Generic runtime error
	GCTK_ERROR_RUNTIME
} ErrorCode;

typedef enum MessageType {
	GCTK_MESSAGE_INFO,
	GCTK_MESSAGE_WARNING,
	GCTK_MESSAGE_ERROR,
	GCTK_MESSAGE_FATAL_ERROR
} MessageType;

typedef struct Date {
	struct tm* src_time;
	int year, month, day, hours, minutes, seconds;
} Date;

/**
 * Get current date.
 */
GCTK_API Date GctkDateNow();
/**
 * Get current date in UTC/GMT.
 */
GCTK_API Date GctkDateNowUTC();
/**
 * @brief Close debug log file stream.
 */
GCTK_API void GctkCloseDebugLog();
/**
 * @brief Log a debug message.
 * @param caller_file File name that called this function. (Use __FILE__ macro)
 * @param caller_func Function name that called this function. (Use __func__ macro)
 * @param caller_line Line number where this function got called. (Use __LINE__ macro)
 * @param type Type of the message.
 * @param format Message format.
 * @param ... Message format arguments.
 */
GCTK_API void GctkDebugLog(const char* caller_file, const char* caller_func, size_t caller_line, MessageType type, const char* format, ...);
/**
 * @brief Log a debug message.
 * @param caller_file File name that called this function. (Use __FILE__ macro)
 * @param caller_func Function name that called this function. (Use __func__ macro)
 * @param caller_line Line number where this function got called. (Use __LINE__ macro)
 * @param type Type of the message.
 * @param format Message format.
 * @param args Message format arguments.
 */
GCTK_API void GctkDebugLogV(const char* caller_file, const char* caller_func, size_t caller_line, MessageType type, const char* format, va_list args);
/**
 * @brief Set error message and code.
 * @param error Error code.
 * @param format Error message format.
 * @param ... Error message format arguments.
 */
GCTK_API void GctkSetError(ErrorCode error, const char* format, ...);
/**
 * @brief Set error message and code.
 * @param error Error code.
 * @param format Error message format.
 * @param args Error message format arguments.
 */
GCTK_API void GctkSetErrorV(ErrorCode error, const char* format, va_list args);
/**
 * @brief Get current error code and message.
 * @param message Pointer to the output string for error message. If you only need the error code, set this to NULL.
 * @return Returns the error code.
 */
GCTK_API ErrorCode GctkGetError(const char** message);
/***
 * @brief Signal a crash. The game will close and copy over the crash log.
 */
GCTK_API void GctkCrash();

#define GCTK_GET_DEBUG_TRACE __FILE__, __func__, __LINE__
/**
 * @brief Log debug message.
 * @param __format__ Format string.
 * @param ... Format arguments.
 */
#define GCTK_LOG(__format__, ...) GctkDebugLog(GCTK_GET_DEBUG_TRACE, GCTK_MESSAGE_INFO, __format__, ##__VA_ARGS__)
/**
 * @brief Log debug warning message.
 * @param __format__ Format string.
 * @param ... Format arguments.
 */
#define GCTK_LOG_WARN(__format__, ...) GctkDebugLog(GCTK_GET_DEBUG_TRACE, GCTK_MESSAGE_WARNING, __format__, ##__VA_ARGS__)
/**
 * @brief Log debug error message.
 * @param __code__ Error code.
 * @param __format__ Format string.
 * @param ... Format arguments.
 */
#define GCTK_LOG_ERR(__code__, __format__, ...) GctkDebugLog(GCTK_GET_DEBUG_TRACE, GCTK_MESSAGE_ERROR, __format__, ##__VA_ARGS__); \
												GctkSetError(__code__, __format__, ##__VA_ARGS__)
/**
 * @brief Log debug error message, then signal crash.
 * @param __code__ Error code.
 * @param __format__ Format string.
 * @param ... Format arguments.
 */
#define GCTK_LOG_FATAL_ERR(__code__, __format__, ...) { GctkDebugLog(GCTK_GET_DEBUG_TRACE, GCTK_MESSAGE_FATAL_ERROR, __format__, ##__VA_ARGS__); \
													  GctkSetError(__code__, __format__, ##__VA_ARGS__); GctkCrash(); }

#define GCTK_DATE_FORMAT "%u.%u.%u %u:%u:%u"
#define GCTK_DATE_SPREAD(__date__) (__date__).year, (__date__).month, (__date__).day, (__date__).hours, (__date__).minutes, (__date__).seconds