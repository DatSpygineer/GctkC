#pragma once

#include "gctk_common.h"

#ifdef _WIN32
	#include <Windows.h>
#else
	#error "TODO: Implement for linux"
#endif

typedef enum {
	GCTK_MSGBTN_OK,
	GCTK_MSGBTN_OK_CANCEL,
	GCTK_MSGBTN_YES_NO,
	GCTK_MSGBTN_YES_NO_CANCEL
} MessageBoxButtonType;

typedef enum {
	GCTK_MSGRES_ERROR = -1,
	GCTK_MSGRES_OK,
	GCTK_MSGRES_CANCEL,
	GCTK_MSGRES_YES,
	GCTK_MSGRES_NO,
} MessageBoxResult;

typedef enum {
	GCTK_MSGICON_INFO,
	GCTK_MSGICON_WARNING,
	GCTK_MSGICON_ERROR,
	GCTK_MSGICON_QUESTION
} MessageBoxIcon;

GCTK_API MessageBoxResult GctkMessageBox(const char* title, const char* message, MessageBoxIcon icon, MessageBoxButtonType buttons);
GCTK_API MessageBoxResult GctkMessageBoxF(const char* title, MessageBoxIcon icon, MessageBoxButtonType buttons, const char* format, ...);