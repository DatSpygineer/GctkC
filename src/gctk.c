#include "gctk.h"

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL2/SOIL2.h>

#ifdef _WIN32
extern DWORD GctkDefaultConsoleColor;
#endif

static struct {
	GLFWwindow* pWindow;
	int iMonitor;
	bool bFullscreen;
	char szName[64];
	char szAuthor[64];
	char szBasePath[512];
	Color cBackground;
	double fUpdateFrequency;
	double fLastTime, fLastUpdateTime;
	const char* szLuaMainSrc;
} GameInstance = {
	.pWindow = NULL,
	.iMonitor = -1,
	.bFullscreen = false,
	.szName = { 0 },
	.szAuthor = { 0 },
	.szBasePath = { 0 },
	.cBackground = { 0.25f, 0.5f, 1.0f, 1.0f },
	1.0 / 60.0,
	0.0, 0.0,
	NULL
};

extern lua_State* GctkLuaState;

GctkStandardCallbacks GctkCallbacks = { NULL };

ErrorCode GctkInit(const char* name, const char* author, int argc, char** argv) {
	errno = 0; // Make sure error code is 0 at start.

#ifdef _WIN32
	CONSOLE_SCREEN_BUFFER_INFO con_info;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &con_info);
	GctkDefaultConsoleColor = con_info.wAttributes;
#endif

	strcpy(GameInstance.szName, name);
	strcpy(GameInstance.szAuthor, author);

	char* p = strrchr(argv[0], '/');
#ifdef _WIN32
	char* p2 = strrchr(argv[0], '\\');
	if (p2 > p) {
		p = p2;
	}
#endif
	strncpy(GameInstance.szBasePath, argv[0], p - argv[0]);

	GctkSetupPaths(name, author);

	if (glfwInit() == GLFW_FALSE) {
		const char* errmsg;
		GLenum errcode = glfwGetError(&errmsg);
		GCTK_LOG_ERR(GCTK_ERROR_GLFW_INIT, "Failed to initialize GLFW: %s, error code: %u", errmsg, errcode);
		return GCTK_ERROR_GLFW_INIT;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GameInstance.pWindow = glfwCreateWindow(640, 480, name, NULL, NULL);
	if (GameInstance.pWindow == NULL) {
		const char* errmsg;
		GLenum errcode = glfwGetError(&errmsg);
		GCTK_LOG_ERR(GCTK_ERROR_GLFW_INIT, "Failed to create window: %s, error code: %u", errmsg, errcode);
		return GCTK_ERROR_WINDOW_INIT;
	}
	glfwMakeContextCurrent(GameInstance.pWindow);

	if (GctkFileExists("res/icon.png")) {
		GLFWimage icon;
		icon.pixels = SOIL_load_image("res/icon.png", &icon.width, &icon.height, NULL, SOIL_LOAD_RGBA);
		glfwSetWindowIcon(GameInstance.pWindow, 1, &icon);
		SOIL_free_image_data(icon.pixels);
	} else {
		GCTK_LOG("Icon not found! (res/icon.png)");
	}

	GLenum errcode;
	if ((errcode = glewInit()) != GLEW_OK) {
		GCTK_LOG_ERR(GCTK_ERROR_GLEW_INIT, "Failed to initialize GLEW: %s, error code: %u", glewGetErrorString(errcode), errcode);
		glfwDestroyWindow(GameInstance.pWindow);
		glfwTerminate();
		return GCTK_ERROR_GLEW_INIT;
	}

	if (!GctkInitLua()) {
		ErrorCode error = GctkGetError(NULL);
		if (error == GCTK_NO_ERROR) {
			return GCTK_ERROR_LUA_INIT;
		}
		return error;
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#if defined(GCTK_DEBUG) && (GCTK_DEBUG != 0)
	char debug_title[128] = { 0 };
	snprintf(debug_title, 128, "%s [DEBUG] GCTk v%s OpenGL v%s",
			 name,
			 "0.1",
			 glGetString(GL_VERSION)
	);
	glfwSetWindowTitle(GameInstance.pWindow, debug_title);
#endif

	if (GctkCallbacks.pOnLoaded != NULL) GctkCallbacks.pOnLoaded();

	return GCTK_NO_ERROR;
}
bool GctkUpdate() {
	glfwPollEvents();
	glClearColor(ColorSpread(GameInstance.cBackground));
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	double time = glfwGetTime();
	if (time >= GameInstance.fLastUpdateTime + GameInstance.fUpdateFrequency) {
		if (GctkCallbacks.pOnUpdate != NULL) GctkCallbacks.pOnUpdate(time - GameInstance.fLastUpdateTime);
		GameInstance.fLastUpdateTime = time;
	}

	if (GctkCallbacks.pOnRender != NULL) GctkCallbacks.pOnRender(time - GameInstance.fLastTime);
	GameInstance.fLastTime = time;

	glfwSwapBuffers(GameInstance.pWindow);
	return !glfwWindowShouldClose(GameInstance.pWindow);
}
ErrorCode GctkQuit() {
	GctkCloseLua();
	if (GameInstance.pWindow != NULL) {
		glfwDestroyWindow(GameInstance.pWindow);
	}
	glfwTerminate();

	GctkCloseDebugLog();
	return GctkGetError(NULL);
}

void GctkSetupCallbacks(GctkCallbackFunction on_init, GctkFrameCallbackFunction on_update,
						GctkFrameCallbackFunction on_render, GctkCallbackFunction on_close,
						GctkErrorCallbackFunction on_error) {
	GctkCallbacks = (GctkStandardCallbacks) {
			on_init, on_update, on_render, on_close, on_error
	};
}

void GctkSetBackgroundColor(Color color) {
	GameInstance.cBackground = color;
}
Color GctkGetBackgroundColor() {
	return GameInstance.cBackground;
}

void GctkSetUpdateFrequency(double freq) {
	GameInstance.fUpdateFrequency = freq;
}
void GctkSetLuaMainSource(const char* src) {
	GameInstance.szLuaMainSrc = src;
}

const char* GctkGameName() {
	return GameInstance.szName;
}
const char* GctkGameAuthor() {
	return GameInstance.szAuthor;
}
const char* GctkBaseDir() {
	return GameInstance.szBasePath;
}

double GctkTime() {
	return glfwGetTime();
}
void GctkClose() {
	glfwSetWindowShouldClose(GameInstance.pWindow, GLFW_TRUE);
}