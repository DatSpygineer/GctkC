#include "gctk_common.h"

#include "gctk_lua.h"
#include "gctk_math.h"

static int GctkLuaFnVec2New(lua_State* L);
static int GctkLuaFnVec2UnitX(lua_State* L);
static int GctkLuaFnVec2UnitY(lua_State* L);
static int GctkLuaFnVec2Zero(lua_State* L);
static int GctkLuaFnVec2One(lua_State* L);
static int GctkLuaFnVec2Random(lua_State* L);
static int GctkLuaFnVec2Lerp(lua_State* L);

static int GctkLuaFnVec2Add(lua_State* L);
static int GctkLuaFnVec2Sub(lua_State* L);
static int GctkLuaFnVec2Mul(lua_State* L);
static int GctkLuaFnVec2Div(lua_State* L);
static int GctkLuaFnVec2Eq(lua_State* L);
static int GctkLuaFnVec2Neg(lua_State* L);
static int GctkLuaFnVec2ToStr(lua_State* L);

static int GctkLuaFnVec2Len(lua_State* L);
static int GctkLuaFnVec2Dot(lua_State* L);
static int GctkLuaFnVec2Dist(lua_State* L);
static int GctkLuaFnVec2Norm(lua_State* L);
static int GctkLuaFnVec2Items(lua_State* L);

GCTK_API void GctkLuaRegisterVec2(lua_State* L);
GCTK_API Vec2 GctkLuaGetVec2(lua_State* L, int idx);
GCTK_API void GctkLuaPushVec2(lua_State* L, Vec2 vec);

Vec2 GctkLuaGetVec2(lua_State* L, int idx) {
	lua_getfield(L, idx, "x");
	float x = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);
	lua_getfield(L, idx, "y");
	float y = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);

	return Vec2Create(x, y);
}
void GctkLuaPushVec2(lua_State* L, Vec2 vec) {
	lua_newtable(L);
	lua_pushnumber(L, vec.x);
	lua_setfield(L, -2, "x");
	lua_pushnumber(L, vec.y);
	lua_setfield(L, -2, "y");

	lua_pushcfunction(L, &GctkLuaFnVec2Len);
	lua_setfield(L, -2, "length");
	lua_pushcfunction(L, &GctkLuaFnVec2Dot);
	lua_setfield(L, -2, "dot");
	lua_pushcfunction(L, &GctkLuaFnVec2Dist);
	lua_setfield(L, -2, "distance");
	lua_pushcfunction(L, &GctkLuaFnVec2Norm);
	lua_setfield(L, -2, "normalize");
	lua_pushcfunction(L, &GctkLuaFnVec2Items);
	lua_setfield(L, -2, "items");

	lua_getglobal(L, "Vec2Meta");
	lua_setmetatable(L, -2);
}

void GctkLuaRegisterVec2(lua_State* L) {
	lua_newtable(L);
	lua_newtable(L);

	lua_pushcfunction(L, &GctkLuaFnVec2Add);
	lua_setfield(L, -2, "__add");
	lua_pushcfunction(L, &GctkLuaFnVec2Sub);
	lua_setfield(L, -2, "__sub");
	lua_pushcfunction(L, &GctkLuaFnVec2Mul);
	lua_setfield(L, -2, "__mul");
	lua_pushcfunction(L, &GctkLuaFnVec2Div);
	lua_setfield(L, -2, "__div");
	lua_pushcfunction(L, &GctkLuaFnVec2Eq);
	lua_setfield(L, -2, "__eq");
	lua_pushcfunction(L, &GctkLuaFnVec2ToStr);
	lua_setfield(L, -2, "__tostring");
	lua_pushcfunction(L, &GctkLuaFnVec2Len);
	lua_setfield(L, -2, "__len");
	lua_pushcfunction(L, &GctkLuaFnVec2Neg);
	lua_setfield(L, -2, "__unm");
	lua_pushstring(L, "Vec2");
	lua_setfield(L, -2, "__name");
	lua_pushstring(L, "Vec2");
	lua_setfield(L, -2, "__classname");

	lua_setglobal(L, "Vec2Meta");

	lua_pushcfunction(L, &GctkLuaFnVec2New);
	lua_setfield(L, -2, "New");
	lua_pushcfunction(L, &GctkLuaFnVec2UnitX);
	lua_setfield(L, -2, "UnitX");
	lua_pushcfunction(L, &GctkLuaFnVec2UnitY);
	lua_setfield(L, -2, "UnitY");
	lua_pushcfunction(L, &GctkLuaFnVec2Zero);
	lua_setfield(L, -2, "Zero");
	lua_pushcfunction(L, &GctkLuaFnVec2One);
	lua_setfield(L, -2, "One");
	lua_pushcfunction(L, &GctkLuaFnVec2Random);
	lua_setfield(L, -2, "Random");
	lua_pushcfunction(L, &GctkLuaFnVec2Lerp);
	lua_setfield(L, -2, "Lerp");
	lua_setglobal(L, "Vec2");
}

static int GctkLuaFnVec2New(lua_State* L) {
	lua_Number x;
	lua_Number y;

	if (lua_gettop(L) <= 0) {
		x = y = 0;
	} else if (lua_gettop(L) == 1) {
		x = y = luaL_checknumber(L, 1);
	} else {
		x = luaL_checknumber(L, 1);
		y = luaL_checknumber(L, 2);
	}

	GctkLuaPushVec2(L, Vec2Create(x, y));
	return 1;
}
static int GctkLuaFnVec2UnitX(lua_State* L) {
	GctkLuaPushVec2(L, VEC2_UNIT_X);
	return 1;
}
static int GctkLuaFnVec2UnitY(lua_State* L) {
	GctkLuaPushVec2(L, VEC2_UNIT_Y);
	return 1;
}
static int GctkLuaFnVec2Zero(lua_State* L) {
	GctkLuaPushVec2(L, VEC2_ZERO);
	return 1;
}
static int GctkLuaFnVec2One(lua_State* L) {
	GctkLuaPushVec2(L, VEC2_ONE);
	return 1;
}
static int GctkLuaFnVec2Random(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	GctkLuaPushVec2(L, RandomVec2(GctkLuaGetVec2(L, 1), GctkLuaGetVec2(L, 2)));
	return 1;
}
static int GctkLuaFnVec2Lerp(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	GctkLuaPushVec2(L, Vec2Lerp(GctkLuaGetVec2(L, 1), GctkLuaGetVec2(L, 2), (float)luaL_checknumber(L, 3)));
	return 1;
}

static int GctkLuaFnVec2Add(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	GctkLuaPushVec2(L, Vec2Add(GctkLuaGetVec2(L, 1), GctkLuaGetVec2(L, 2)));
	return 1;
}
static int GctkLuaFnVec2Sub(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	GctkLuaPushVec2(L, Vec2Sub(GctkLuaGetVec2(L, 1), GctkLuaGetVec2(L, 2)));
	return 1;
}
static int GctkLuaFnVec2Mul(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	if (lua_istable(L, 1)) {
		GctkLuaPushVec2(L, Vec2Mul(GctkLuaGetVec2(L, 1), GctkLuaGetVec2(L, 2)));
	} else {
		GctkLuaPushVec2(L, Vec2MulScalar(GctkLuaGetVec2(L, 1), (float)luaL_checknumber(L, 2)));
	}
	return 1;
}
static int GctkLuaFnVec2Div(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	if (lua_istable(L, 1)) {
		GctkLuaPushVec2(L, Vec2Div(GctkLuaGetVec2(L, 1), GctkLuaGetVec2(L, 2)));
	} else {
		GctkLuaPushVec2(L, Vec2DivScalar(GctkLuaGetVec2(L, 1), (float)luaL_checknumber(L, 2)));
	}
	return 1;
}
static int GctkLuaFnVec2Eq(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	lua_pushboolean(L, Vec2Eq(GctkLuaGetVec2(L, 1), GctkLuaGetVec2(L, 2)));
	return 1;
}
static int GctkLuaFnVec2Neg(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	GctkLuaPushVec2(L, Vec2Neg(GctkLuaGetVec2(L, 1)));
	return 1;
}
static int GctkLuaFnVec2ToStr(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	Vec2 v = GctkLuaGetVec2(L, 1);
	char result[64] = { 0 };
	snprintf(result, 64, "%f, %f", v.x, v.y);
	lua_pushstring(L, result);

	return 1;
}

static int GctkLuaFnVec2Len(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_pushnumber(L, Vec2Length(GctkLuaGetVec2(L, 1)));
	return 1;
}
static int GctkLuaFnVec2Dot(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	lua_pushnumber(L, Vec2Dot(GctkLuaGetVec2(L, 1), GctkLuaGetVec2(L, 2)));
	return 1;
}
static int GctkLuaFnVec2Dist(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	lua_pushnumber(L, Vec2Dist(GctkLuaGetVec2(L, 1), GctkLuaGetVec2(L, 2)));
	return 1;
}
static int GctkLuaFnVec2Norm(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	GctkLuaPushVec2(L, Vec2Norm(GctkLuaGetVec2(L, 1)));
	return 1;
}
static int GctkLuaFnVec2Items(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	Vec2 v = GctkLuaGetVec2(L, 1);
	lua_pushnumber(L, v.x);
	lua_pushnumber(L, v.y);
	return 2;
}