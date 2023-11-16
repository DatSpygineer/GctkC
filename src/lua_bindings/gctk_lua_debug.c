#include "gctk_common.h"
#include "gctk_lua.h"
#include "gctk_debug.h"

GCTK_API void GctkLuaRegisterDebug(lua_State* L);

static int GctkLuaFnLog(lua_State* L) {
	lua_Debug debug;
	lua_getstack(L, 1, &debug);
	lua_getinfo(L, "nSl", &debug);

	char name[128] = { 0 };
	snprintf(name, 128, "Lua code \"%s\"", debug.what);

	GctkDebugLog(name, debug.name, debug.currentline, GCTK_MESSAGE_INFO, "%s", luaL_checkstring(L, 1));
	return 0;
}
static int GctkLuaFnLogWarn(lua_State* L) {
	lua_Debug debug;
	lua_getstack(L, 1, &debug);
	lua_getinfo(L, "nSl", &debug);

	char name[128] = { 0 };
	snprintf(name, 128, "Lua code \"%s\"", debug.what);

	GctkDebugLog(name, debug.name, debug.currentline, GCTK_MESSAGE_WARNING, "%s", luaL_checkstring(L, 1));
	return 0;
}
static int GctkLuaFnLogError(lua_State* L) {
	lua_Debug debug;
	lua_getstack(L, 1, &debug);
	lua_getinfo(L, "nSl", &debug);

	char name[128] = { 0 };
	snprintf(name, 128, "Lua code \"%s\"", debug.what);

	GctkDebugLog(name, debug.name, debug.currentline, GCTK_MESSAGE_ERROR, "%s", luaL_checkstring(L, 1));
	GctkSetError(GCTK_ERROR_LUA_RUNTIME, "%s", luaL_checkstring(L, 1));
	return 0;
}
static int GctkLuaFnLogFatal(lua_State* L) {
	lua_Debug debug;
	lua_getstack(L, 1, &debug);
	lua_getinfo(L, "nSl", &debug);

	char name[128] = { 0 };
	snprintf(name, 128, "Lua code \"%s\"", debug.what);

	GctkDebugLog(name, debug.name, debug.currentline, GCTK_MESSAGE_ERROR, "%s", luaL_checkstring(L, 1));
	GctkSetError(GCTK_ERROR_LUA_RUNTIME, "%s", luaL_checkstring(L, 1));
	GctkCrash();
	return 0;
}

void GctkLuaRegisterDebug(lua_State* L) {
	lua_newtable(L);

	lua_pushcfunction(L, &GctkLuaFnLog);
	lua_setfield(L, -2, "Log");
	lua_pushcfunction(L, &GctkLuaFnLogWarn);
	lua_setfield(L, -2, "Warning");
	lua_pushcfunction(L, &GctkLuaFnLogError);
	lua_setfield(L, -2, "Error");
	lua_pushcfunction(L, &GctkLuaFnLogFatal);
	lua_setfield(L, -2, "FatalError");

	lua_setglobal(L, "Debug");
}