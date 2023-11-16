#include "gctk_common.h"

#include "gctk_lua.h"
#include "gctk_math.h"

GCTK_API void GctkLuaRegisterColor(lua_State* L);
GCTK_API Color GctkLuaGetColor(lua_State* L, int idx);
GCTK_API void GctkLuaPushColor(lua_State* L, Color color);

static int GctkLuaFnColorNew(lua_State* L);
static int GctkLuaFnColorFromBytes(lua_State* L);
static int GctkLuaFnColorFromHex(lua_State* L);

static int GctkLuaFnColorEq(lua_State* L);
static int GctkLuaFnColorToStr(lua_State* L);

static int GctkLuaFnColorToHex(lua_State* L);
static int GctkLuaFnColorItems(lua_State* L);

void GctkLuaRegisterColor(lua_State* L) {
	lua_newtable(L);
	lua_pushstring(L, "Color");
	lua_setfield(L, -2, "__classname");
	lua_pushcfunction(L, &GctkLuaFnColorEq);
	lua_setfield(L, -2, "__eq");
	lua_pushcfunction(L, &GctkLuaFnColorToStr);
	lua_setfield(L, -2, "__tostring");
	lua_setglobal(L, "ColorMeta");

	lua_newtable(L);
	lua_pushcfunction(L, &GctkLuaFnColorNew);
	lua_setfield(L, -2, "New");
	lua_pushcfunction(L, &GctkLuaFnColorFromBytes);
	lua_setfield(L, -2, "FromBytes");
	lua_pushcfunction(L, &GctkLuaFnColorFromHex);
	lua_setfield(L, -2, "FromHex");
	lua_setglobal(L, "Color");
}

Color GctkLuaGetColor(lua_State* L, int idx) {
	lua_getfield(L, idx, "r");
	float r = lua_tonumber(L, -1);
	lua_pop(L, 1);
	lua_getfield(L, idx, "g");
	float g = lua_tonumber(L, -1);
	lua_pop(L, 1);
	lua_getfield(L, idx, "b");
	float b = lua_tonumber(L, -1);
	lua_pop(L, 1);
	lua_getfield(L, idx, "a");
	float a = lua_tonumber(L, -1);
	lua_pop(L, 1);
	return ColorCreate(r, g, b, a);
}
void GctkLuaPushColor(lua_State* L, Color color) {
	lua_newtable(L);
	lua_pushnumber(L, color.r);
	lua_setfield(L, -1, "r");
	lua_pushnumber(L, color.g);
	lua_setfield(L, -1, "g");
	lua_pushnumber(L, color.b);
	lua_setfield(L, -1, "b");
	lua_pushnumber(L, color.a);
	lua_setfield(L, -1, "a");

	lua_pushcfunction(L, &GctkLuaFnColorToHex);
	lua_setfield(L, -1, "toHex");
	lua_pushcfunction(L, &GctkLuaFnColorItems);
	lua_setfield(L, -1, "items");
	lua_getglobal(L, "ColorMeta");
	lua_setmetatable(L, -2);
}

static int GctkLuaFnColorNew(lua_State* L) {
	float r = 0, g = 0, b = 0, a = 1;
	if (lua_gettop(L) >= 3) {
		r = lua_tonumber(L, 1);
		g = lua_tonumber(L, 2);
		b = lua_tonumber(L, 3);
		if (lua_gettop(L) > 3) {
			a = lua_tonumber(L, 4);
		}
	} else if (lua_gettop(L) != 0 ) {
		luaL_error(L, "Expected 0, 3 or 4 arguments!");
	}

	GctkLuaPushColor(L, ColorCreate(r, g, b, a));
	return 1;
}
static int GctkLuaFnColorFromBytes(lua_State* L) {
	if (lua_gettop(L) == 1) {
		uint32_t rgba = lua_tointeger(L, 1);
		GctkLuaPushColor(L, ColorFromRgba(rgba));
	} else if (lua_gettop(L) >= 3) {
		uint32_t r = 0, g = 0, b = 0, a = 255;
		r = lua_tointeger(L, 1);
		g = lua_tointeger(L, 2);
		b = lua_tointeger(L, 3);
		if (lua_gettop(L) > 3) {
			a = lua_tointeger(L, 4);
		}
		GctkLuaPushColor(L, ColorFromBytes(r, g, b, a));
	} else {
		luaL_error(L, "Expected 1, 3 or 4 arguments!");
	}

	return 1;
}
static int GctkLuaFnColorFromHex(lua_State* L) {
	GctkLuaPushColor(L, ColorFromHex(luaL_checkstring(L, 1)));
	return 1;
}

static int GctkLuaFnColorEq(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	lua_pushboolean(L, ColorEq(GctkLuaGetColor(L, 1), GctkLuaGetColor(L, 2)));
	return 1;
}
static int GctkLuaFnColorToStr(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	Color col = GctkLuaGetColor(L, 1);
	char temp[192] = { 0 };
	snprintf(temp, 128, "%f, %f, %f, %f", col.r, col.g, col.b, col.a);
	lua_pushstring(L, temp);
	return 1;
}

static int GctkLuaFnColorToHex(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	uint32_t rgba = ColorToRgba(GctkLuaGetColor(L, 1));
	char temp[10] = { 0 };
	snprintf(temp, 10, "#%08X", rgba);
	lua_pushstring(L, temp);
	return 1;
}

static int GctkLuaFnColorItems(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	Color c = GctkLuaGetColor(L, 1);
	lua_pushnumber(L, c.r);
	lua_pushnumber(L, c.g);
	lua_pushnumber(L, c.b);
	lua_pushnumber(L, c.a);

	return 4;
}