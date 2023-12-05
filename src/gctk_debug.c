#include "gctk.h"
#include "gctk_debug.h"
#include "gctk_filesys.h"

extern GctkStandardCallbacks GctkCallbacks;

static char GctkErrorMessage[256] = { 0 };
static ErrorCode GctkErrorCode = GCTK_NO_ERROR;
static FILE* GctkLogFile = NULL;
#ifdef _WIN32
DWORD GctkDefaultConsoleColor = 0;
#endif

static void GctkGetLogPath(char* log_path) {
#if defined(GCTK_DEBUG) && (GCTK_DEBUG != 0)
	strcpy(log_path, GctkBaseDir());
	GctkJoinPaths(log_path, "logs", GCTK_PATH_MAX);
#else
	GctkJoinPaths(log_path, GctkGameName(), GCTK_PATH_MAX);
#endif
	if (!GctkDirExists(log_path)) GctkDirCreate(log_path);
	char log_name[64] = { 0 };
	Date date = GctkDateNow();
	snprintf(log_name, 64, "log_%u.%u.%u.txt",
			 date.year, date.month, date.day
	);
	GctkJoinPaths(log_path, log_name, GCTK_PATH_MAX);
}

static FILE* GctkGetLogFile() {
	if (GctkLogFile == NULL) {
		char log_path[GCTK_PATH_MAX] = { 0 };
		GctkGetLogPath(log_path);

		if (GctkFileExists(log_path)) {
			return fopen(log_path, "a");
		} else {
			return fopen(log_path, "w");
		}
	}
	return GctkLogFile;
}
static void GctkFormatDebugMessage(const char* caller_file, const char* caller_func, size_t caller_line, const char* message,
								   const char* message_type, bool write_log) {
	Date date = GctkDateNow();
	if (write_log) {
		FILE* log_file = GctkGetLogFile();
		if (log_file != NULL) {
			fprintf(log_file, GCTK_DATE_FORMAT " [%s] - %s:" GCTK_SIZE_FORMAT " [%s]: %s\n",
					GCTK_DATE_SPREAD(date), message_type, caller_file, caller_line, caller_func, message
			);
		}
	}

	printf(GCTK_DATE_FORMAT " [%s] - %s:" GCTK_SIZE_FORMAT " [%s]: %s\n",
		GCTK_DATE_SPREAD(date), message_type, caller_file, caller_line, caller_func, message
	);
}

Date GctkDateNow() {
	time_t t = time(NULL);
	struct tm* tm = localtime(&t);
	return (Date) {
		.src_time = tm,
		.year = tm->tm_year + 1900,
		.month = tm->tm_mon + 1,
		.day = tm->tm_wday,
		.hours = tm->tm_hour,
		.minutes = tm->tm_min,
		.seconds = tm->tm_sec
	};
}
Date GctkDateNowUTC() {
	time_t t = time(NULL);
	struct tm* tm = gmtime(&t);
	return (Date) {
		.src_time = tm,
		.year = tm->tm_year + 1900,
		.month = tm->tm_mon + 1,
		.day = tm->tm_wday,
		.hours = tm->tm_hour,
		.minutes = tm->tm_min,
		.seconds = tm->tm_sec
	};
}

void GctkCloseDebugLog() {
	if (GctkLogFile != NULL) {
		fclose(GctkLogFile);
	}
}

void GctkDebugLog(const char* caller_file, const char* caller_func, size_t caller_line, MessageType type, const char* format, ...) {
	va_list args;
	va_start(args, format);
	GctkDebugLogV(caller_file, caller_func, caller_line, type, format, args);
	va_end(args);
}
void GctkDebugLogV(const char* caller_file, const char* caller_func, size_t caller_line, MessageType type, const char* format, va_list args) {
	char message[256] = {0 };
	const char* type_str;
	switch (type) {
		default: case GCTK_MESSAGE_INFO: type_str = "INFO"; break;
		case GCTK_MESSAGE_WARNING: type_str = "WARN"; break;
		case GCTK_MESSAGE_ERROR: type_str = "ERROR"; break;
		case GCTK_MESSAGE_FATAL_ERROR: type_str = "FATAL"; break;
	}

	switch (type) {
		case GCTK_MESSAGE_WARNING: {
#ifdef _WIN32
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
#else
			printf("\x1B[33;1m");
#endif
		} break;
		case GCTK_MESSAGE_ERROR: {
#ifdef _WIN32
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
#else
			printf("\x1B[31m");
#endif
		} break;
		case GCTK_MESSAGE_FATAL_ERROR: {
#ifdef _WIN32
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
#else
			printf("\x1B[31;1m");
#endif
		} break;
		default: break;
	}

	vsnprintf(message, 256, format, args);
	GctkFormatDebugMessage(caller_file, caller_func, caller_line, message, type_str, type != GCTK_MESSAGE_WARNING);

	if (type != GCTK_MESSAGE_INFO) {
#ifdef _WIN32
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GctkDefaultConsoleColor);
#else
		printf("\x1B[0m");
#endif
	}
}

void GctkSetError(ErrorCode error, const char* format, ...) {
	va_list args;
	va_start(args, format);
	GctkSetErrorV(error, format, args);
	va_end(args);
}
void GctkSetErrorV(ErrorCode error, const char* format, va_list args) {
	GctkErrorCode = error;
	snprintf(GctkErrorMessage, 256, format, args);
	if (GctkCallbacks.pOnError != NULL) GctkCallbacks.pOnError(GctkErrorCode, GctkErrorMessage);
}

ErrorCode GctkGetError(const char** message) {
	if (message != NULL) {
		*message = GctkErrorMessage;
	}
	return GctkErrorCode;
}

void GctkCrash() {
	GctkDebugLog(GCTK_GET_DEBUG_TRACE, GCTK_MESSAGE_FATAL_ERROR, "GAME CRASH TRIGGERED!!");

	Date date = GctkDateNow();
	char temp[64] = { 0 };
	char dest_path[GCTK_PATH_MAX] = { 0 };
	char src_path[GCTK_PATH_MAX] = { 0 };
	GctkGetLogPath(src_path);

	strcpy(dest_path, GctkBaseDir());
	GctkJoinPaths(dest_path, "crash", GCTK_PATH_MAX);
	if (!GctkDirExists(dest_path)) GctkDirCreate(dest_path);
	snprintf(temp, 64, "crash_%u.%u.%u_%u-%u-%u.txt",
			date.year, date.month, date.day, date.hours, date.minutes, date.seconds
	);
	GctkJoinPaths(dest_path, temp, GCTK_PATH_MAX);
	GctkMoveFile(src_path, dest_path);

	const MessageBoxResult res = GctkMessageBoxF("Fatal error!", GCTK_MSGICON_ERROR, GCTK_MSGBTN_YES_NO,
		"Engine has encountered a fatal error! Crashlog copied over to \"%s\".\nPlease report this to the developer!\nWould you like to open the crash log folder?",
		dest_path
	);
	if (res == GCTK_MSGRES_YES) {
		memset(dest_path, 0, GCTK_PATH_MAX);
		strcpy(dest_path, GctkBaseDir());
		GctkJoinPaths(dest_path, "crash", GCTK_PATH_MAX);

		char cmd[512] = { 0 };
#ifdef _WIN32
		snprintf(cmd, 512, "explorer \"%s\"", dest_path);
#else
		snprintf(cmd, 512, "xdg-open \"%s\"", dest_path);
#endif
		system(cmd);
	}

	GctkClose();
}
