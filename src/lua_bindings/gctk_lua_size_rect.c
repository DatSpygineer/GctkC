#include "gctk_common.h"
#include "gctk_lua.h"
#include "gctk_shader.h"
#include "gctk_string.h"

static int GctkLuaFnSizeNew(lua_State* L);
static int GctkLuaFnSizeFromVec2(lua_State* L);

static int GctkLuaFnRectNew(lua_State* L);

static int GctkLuaFnSizeEq(lua_State* L);
static int GctkLuaFnRectEq(lua_State* L);

static int GctkLuaFnSizeToVec2(lua_State* L);

static int GctkLuaFnRectCollisionWithPoint(lua_State* L);
static int GctkLuaFnRectCollisionWithRect(lua_State* L);

extern Vec2 GctkLuaGetVec2(lua_State* L, int idx);
extern void GctkLuaPushVec2(lua_State* L, Vec2 vec);

GCTK_API void GctkLuaPushSize(lua_State* L, Size size);
GCTK_API Size GctkLuaGetSize(lua_State* L, int idx);
GCTK_API void GctkLuaPushRect(lua_State* L, Rect rect);
GCTK_API Rect GctkLuaGetRect(lua_State* L, int idx);

void GctkLuaPushSize(lua_State* L, Size size) {
	lua_newtable(L);
	lua_pushnumber(L, size.width);
	lua_setfield(L, -2, "width");
	lua_pushnumber(L, size.height);
	lua_setfield(L, -2, "height");

	lua_getglobal(L, "SizeMeta");
	lua_setmetatable(L, -2);
}
Size GctkLuaGetSize(lua_State* L, int idx) {
	lua_getfield(L, idx, "width");
	float width = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);
	lua_getfield(L, idx, "height");
	float height = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);

	return (Size){ .width = width, .height = height };
}

void GctkLuaPushRect(lua_State* L, Rect rect) {
	lua_newtable(L);
	GctkLuaPushVec2(L, rect.location);
	lua_setfield(L, -2, "location");
	GctkLuaPushSize(L, rect.size);
	lua_setfield(L, -2, "size");

	lua_pushcfunction(L, &GctkLuaFnRectCollisionWithPoint);
	lua_setfield(L, -2, "collisionWithPoint");
	lua_pushcfunction(L, &GctkLuaFnRectCollisionWithRect);
	lua_setfield(L, -2, "collisionWithRect");

	lua_getglobal(L, "RectMeta");
	lua_setmetatable(L, -2);
}

static Rect GctkLuaGetRect(lua_State* L, int idx) {
	lua_getfield(L, idx, "location");
	Vec2 location = GctkLuaGetVec2(L, -1);
	lua_pop(L, 1);
	lua_getfield(L, idx, "size");
	Size size = GctkLuaGetSize(L, -1);
	lua_pop(L, 1);
	return (Rect){ .location = location, .size = size };
}
static void GctkLuaRegisterSizeRect(lua_State* L) {

}

static int GctkLuaFnSizeToVec2(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	Size size = GctkLuaGetSize(L, 1);

	GctkLuaPushVec2(L, (Vec2){ size.width, size.height });
	return 1;
}

static int GctkLuaFnRectCollisionWithPoint(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	lua_pushboolean(L, RectCollisionWithPoint(GctkLuaGetRect(L, 1), GctkLuaGetVec2(L, 2)));
	return 1;
}
static int GctkLuaFnRectCollisionWithRect(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	lua_pushboolean(L, RectCollisionWithRect(GctkLuaGetRect(L, 1), GctkLuaGetRect(L, 2)));
	return 1;
}