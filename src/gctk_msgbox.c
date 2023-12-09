#include "gctk_msgbox.h"

#include <stdarg.h>

MessageBoxResult GctkMessageBox(const char* title, const char* message, MessageBoxIcon icon, MessageBoxButtonType buttons) {
#ifdef _WIN32
	int flags = 0;
	switch (icon) {
		default: case GCTK_MSGICON_INFO: flags = MB_ICONINFORMATION; break;
		case GCTK_MSGICON_WARNING: flags = MB_ICONWARNING; break;
		case GCTK_MSGICON_ERROR: flags = MB_ICONERROR; break;
		case GCTK_MSGICON_QUESTION: flags = MB_ICONQUESTION; break;
	}
	switch (buttons) {
		default: case GCTK_MSGBTN_OK: flags |= MB_OK; break;
		case GCTK_MSGBTN_OK_CANCEL: flags |= MB_OKCANCEL; break;
		case GCTK_MSGBTN_YES_NO: flags |= MB_YESNO; break;
		case GCTK_MSGBTN_YES_NO_CANCEL: flags |= MB_YESNOCANCEL; break;
	}

	int res = MessageBoxA(NULL, message, title, flags);
	if (res != 0) {
		switch (res) {
			case IDOK: return GCTK_MSGRES_OK;
			case IDCANCEL: return GCTK_MSGRES_CANCEL;
			case IDYES: return GCTK_MSGRES_YES;
			case IDNO: return GCTK_MSGRES_NO;
			default: /* Do nothing */ break;
		}
	}
#else
	// TODO: Add message box for linux
#endif
	return GCTK_MSGRES_ERROR;
}
MessageBoxResult GctkMessageBoxF(const char* title, MessageBoxIcon icon, MessageBoxButtonType buttons, const char* format, ...) {
	char message[2048] = { 0 };
	va_list args;
	va_start(args, format);
	vsnprintf(message, 2048, format, args);
	va_end(args);
	return GctkMessageBox(title, message, icon, buttons);
}