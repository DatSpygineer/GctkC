#include "gctk_filesys.h"

#ifdef _WIN32
	#include <shlobj_core.h>
	#include <direct.h>
#else
	#include <fcntl.h>
#if defined(__APPLE__) || defined(__FreeBSD__)
	#include <copyfile.h>
#else
	#include <sys/sendfile.h>
#endif
#endif

extern const char* GctkBaseDir();

static char GctkUserDirectory[GCTK_PATH_MAX] = { 0 };
static char GctkTempDirectory[GCTK_PATH_MAX] = { 0 };

bool GctkSetupPaths(const char* name, const char* author) {
#ifdef _WIN32
	SHGetFolderPathA(NULL, CSIDL_MYDOCUMENTS | CSIDL_FLAG_CREATE, NULL, 0, GctkUserDirectory);
	GetTempPathA(GCTK_PATH_MAX, GctkTempDirectory);
#else
	struct passwd* pwd = getpwuid(getuid());
	strcpy(GctkUserDirectory, pwd->pw_dir);
	strcpy(GctkTempDirectory, "/tmp");
#endif
#ifndef _WIN32
	if (GctkJoinPaths(GctkUserDirectory, "Games", GCTK_PATH_MAX)) {
		return false;
	}
	if (!GctkDirExists(GctkUserDirectory)) { GctkDirCreate(GctkUserDirectory); }
#endif
	if (GctkJoinPaths(GctkUserDirectory, author, GCTK_PATH_MAX)) {
		return false;
	}
	if (!GctkDirExists(GctkUserDirectory)) { GctkDirCreate(GctkUserDirectory); }
	if (GctkJoinPaths(GctkUserDirectory, name, GCTK_PATH_MAX)) {
		return false;
	}
	if (!GctkDirExists(GctkUserDirectory)) { GctkDirCreate(GctkUserDirectory); }

	if (GctkJoinPaths(GctkTempDirectory, name, GCTK_PATH_MAX)) {
		return false;
	}
	if (!GctkDirExists(GctkTempDirectory)) { GctkDirCreate(GctkTempDirectory); }

	return true;
}

errno_t GctkJoinPaths(char* dest, const char* src, size_t buffer_max) {
	const size_t dest_len = strlen(dest);
	if (dest[dest_len - 1] == '/' || src[0] == '/'
#ifdef _WIN32
		|| dest[dest_len - 1] == '\\' || src[0] == '\\'
#endif
	) {
		return GctkStrCat(dest, src, buffer_max);
	}

	errno_t err;
	if ((err = GctkStrCat(dest, GCTK_PATH_SEP_STR, buffer_max)) != 0) {
		return err;
	}
	return GctkStrCat(dest, src, buffer_max);
}

bool GctkFileExists(const char* path) {
	struct stat st;
	return stat(path, &st) == 0 && S_ISREG(st.st_mode);
}
bool GctkDirExists(const char* path) {
	struct stat st;
	return stat(path, &st) == 0 && S_ISDIR(st.st_mode);
}

bool GctkDirCreate(const char* path) {
#ifdef _WIN32
	return _mkdir(path) == 0;
#else
	return mkdir(path, 0777) == 0;
#endif
}

bool GctkCopyFile(const char* from, const char* to, bool allow_overwrite) {
#ifdef _WIN32
	return CopyFileA(from, to, !allow_overwrite);
#else
	int src = open(from, O_RDONLY);
	if (src < 0) return false;
	int dest = creat(to, 0666);
	if (dest < 0 && allow_overwrite) {
		dest = open(to, O_WRONLY);
	}
	if (dest < 0) {
		close(src);
		return false;
	}

#if defined(__APPLE__) || defined(__FreeBSD__)
	bool result = fcopyfile(src, dest, 0, COPYFILE_ALL) == 0;
#else
	off_t count = 0;
	struct stat info;
	fstat(src, &info);
	bool result = sendfile(src, dest, &count, info.st_size);
#endif

	close(src);
	close(dest);

	return result;
#endif
}
bool GctkMoveFile(const char* from, const char* to) {
#ifdef _WIN32
	return MoveFileA(from, to);
#else
	return rename(from, to) == 0;
#endif
}

const char* GctkUserDir() {
	return GctkUserDirectory;
}
const char* GctkTempDir() {
	return GctkTempDirectory;
}