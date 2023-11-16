#include "gctk_lua.h"
#include "gctk_math.h"
#include "gctk_debug.h"

extern void GctkLuaRegisterVec2(lua_State* L);
extern void GctkLuaRegisterVec3(lua_State* L);
extern void GctkLuaRegisterVec4(lua_State* L);
extern void GctkLuaRegisterQuat(lua_State* L);
extern void GctkLuaRegisterColor(lua_State* L);
extern void GctkLuaRegisterMat4(lua_State* L);
extern void GctkLuaRegisterTexture(lua_State* L);
extern void GctkLuaRegisterShader(lua_State* L);
extern void GctkLuaRegisterDebug(lua_State* L);
extern void GctkLuaRegisterMath(lua_State* L);

lua_State* GctkLuaState;

static int GctkLuaFnTypeName(lua_State* L) {
	if (lua_istable(L, 1)) {
		lua_getmetatable(L, 1);
		if (lua_isnil(L, -1)) {
			lua_pop(L, 1);
			lua_pushstring(L, lua_typename(L, 1));
		} else {
			lua_getfield(L, -1, "__classname");
			if (lua_isnil(L, -1)) {
				lua_pop(L, 2);
				lua_pushstring(L, lua_typename(L, 1));
			} else {
				lua_remove(L, -2);
			}
		}
	} else {
		lua_pushstring(L, lua_typename(L, 1));
	}

	return 1;
}
static int GctkLuaFnIsBoolean(lua_State* L) {
	lua_pushboolean(L, lua_isboolean(L, 1));
	return 1;
}
static int GctkLuaFnIsInteger(lua_State* L) {
	lua_pushboolean(L, lua_isinteger(L, 1));
	return 1;
}
static int GctkLuaFnIsNumber(lua_State* L) {
	lua_pushboolean(L, lua_isnumber(L, 1));
	return 1;
}
static int GctkLuaFnIsString(lua_State* L) {
	lua_pushboolean(L, lua_isstring(L, 1));
	return 1;
}
static int GctkLuaFnIsFunction(lua_State* L) {
	lua_pushboolean(L, lua_isfunction(L, 1) || lua_iscfunction(L, 1));
	return 1;
}
static int GctkLuaFnIsTable(lua_State* L) {
	lua_pushboolean(L, lua_istable(L, 1));
	return 1;
}
static int GctkLuaFnIsClass(lua_State* L) {
	if (lua_gettop(L) < 2) {
		lua_pushboolean(L, false);
		luaL_error(L, "Expected two arguments!");
	} else {
		lua_pushboolean(L, (lua_istable(L, 1) || lua_isuserdata(L, 1)) && GctkLuaIsTypeOf(L, 1, lua_tostring(L, 1)));
	}
	return 1;
}
bool GctkInitLua() {
	GctkLuaState = luaL_newstate();
	luaL_openlibs(GctkLuaState);

	lua_pushcfunction(GctkLuaState, &GctkLuaFnTypeName);
	lua_setglobal(GctkLuaState, "typename");
	lua_pushcfunction(GctkLuaState, &GctkLuaFnIsBoolean);
	lua_setglobal(GctkLuaState, "isBool");
	lua_pushcfunction(GctkLuaState, &GctkLuaFnIsInteger);
	lua_setglobal(GctkLuaState, "isInteger");
	lua_pushcfunction(GctkLuaState, &GctkLuaFnIsNumber);
	lua_setglobal(GctkLuaState, "isNumber");
	lua_pushcfunction(GctkLuaState, &GctkLuaFnIsBoolean);
	lua_setglobal(GctkLuaState, "isBoolean");
	lua_pushcfunction(GctkLuaState, &GctkLuaFnIsString);
	lua_setglobal(GctkLuaState, "isString");
	lua_pushcfunction(GctkLuaState, &GctkLuaFnIsFunction);
	lua_setglobal(GctkLuaState, "isFunction");
	lua_pushcfunction(GctkLuaState, &GctkLuaFnIsTable);
	lua_setglobal(GctkLuaState, "isTable");
	lua_pushcfunction(GctkLuaState, &GctkLuaFnIsClass);
	lua_setglobal(GctkLuaState, "isClass");

	GctkLuaRegisterVec2(GctkLuaState);
	GctkLuaRegisterVec3(GctkLuaState);
	GctkLuaRegisterVec4(GctkLuaState);
	GctkLuaRegisterQuat(GctkLuaState);
	GctkLuaRegisterColor(GctkLuaState);
	GctkLuaRegisterMat4(GctkLuaState);
	GctkLuaRegisterTexture(GctkLuaState);
	GctkLuaRegisterShader(GctkLuaState);
	GctkLuaRegisterDebug(GctkLuaState);
	GctkLuaRegisterMath(GctkLuaState);

	return true;
}

void GctkCloseLua() {
	if (GctkLuaState != NULL) {
		lua_close(GctkLuaState);
	}
}

bool GctkLuaIsTypeOf(lua_State* L, int idx, const char* name) {
	if (lua_istable(L, idx) || lua_isuserdata(L, idx)) {
		bool value = false;
		if (lua_getmetatable(L, -1) && lua_getfield(L, -1, "__classname") != LUA_TNIL) {
			value = GctkStrEq(lua_tostring(L, -1), name, true);
			lua_pop(L, 2);
		}
		return value;
	} else {
		return GctkStrEq(lua_typename(L, lua_type(L, idx)), name, false);
	}
}

bool GctkLuaRunString(const char* name, const char* src) {
	char chunk_name[128] = { 0 };
	snprintf(chunk_name, 128, "@%s", name);
	luaL_loadbuffer(GctkLuaState, src, strlen(src), chunk_name);

	if (lua_pcall(GctkLuaState, 0, LUA_MULTRET, 0) != LUA_OK) {
		GCTK_LOG_ERR(GCTK_ERROR_LUA_RUNTIME, "Failed to run Lua code: %s", lua_tostring(GctkLuaState, -1));
		return false;
	}
	return true;
}

bool GctkLuaRunFile(const char* path) {
	if (luaL_dofile(GctkLuaState, path) != LUA_OK) {
		GCTK_LOG_ERR(GCTK_ERROR_LUA_RUNTIME, "Failed to run Lua file \"%s\": %s", path, lua_tostring(GctkLuaState, -1));
		return false;
	}
	return true;
}
