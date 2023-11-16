#include "gctk_common.h"

#include "gctk_lua.h"
#include "gctk_math.h"

static int GctkLuaFnVec3New(lua_State* L);
static int GctkLuaFnVec3FromVec2(lua_State* L);
static int GctkLuaFnVec3UnitX(lua_State* L);
static int GctkLuaFnVec3UnitY(lua_State* L);
static int GctkLuaFnVec3UnitZ(lua_State* L);
static int GctkLuaFnVec3Zero(lua_State* L);
static int GctkLuaFnVec3One(lua_State* L);
static int GctkLuaFnVec3Random(lua_State* L);
static int GctkLuaFnVec3Lerp(lua_State* L);

static int GctkLuaFnVec3Add(lua_State* L);
static int GctkLuaFnVec3Sub(lua_State* L);
static int GctkLuaFnVec3Mul(lua_State* L);
static int GctkLuaFnVec3Div(lua_State* L);
static int GctkLuaFnVec3Eq(lua_State* L);
static int GctkLuaFnVec3Neg(lua_State* L);
static int GctkLuaFnVec3ToStr(lua_State* L);

static int GctkLuaFnVec3Len(lua_State* L);
static int GctkLuaFnVec3Dot(lua_State* L);
static int GctkLuaFnVec3Dist(lua_State* L);
static int GctkLuaFnVec3Cross(lua_State* L);
static int GctkLuaFnVec3Norm(lua_State* L);
static int GctkLuaFnVec3XY(lua_State* L);
static int GctkLuaFnVec3Items(lua_State* L);

GCTK_API void GctkLuaRegisterVec3(lua_State* L);
GCTK_API Vec3 GctkLuaGetVec3(lua_State* L, int idx);
GCTK_API void GctkLuaPushVec3(lua_State* L, Vec3 vec);

extern Vec2 GctkLuaGetVec2(lua_State* L, int idx);
extern void GctkLuaPushVec2(lua_State* L, Vec2 vec);

Vec3 GctkLuaGetVec3(lua_State* L, int idx) {
	lua_getfield(L, idx, "x");
	float x = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);
	lua_getfield(L, idx, "y");
	float y = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);
	lua_getfield(L, idx, "z");
	float z = (float)lua_tonumber(L, -1);
	lua_pop(L, 2);

	return Vec3Create(x, y, z);
}
void GctkLuaPushVec3(lua_State* L, Vec3 vec) {
	lua_newtable(L);
	lua_pushnumber(L, vec.x);
	lua_setfield(L, -2, "x");
	lua_pushnumber(L, vec.y);
	lua_setfield(L, -2, "y");
	lua_pushnumber(L, vec.z);
	lua_setfield(L, -2, "z");

	lua_pushcfunction(L, &GctkLuaFnVec3Len);
	lua_setfield(L, -2, "length");
	lua_pushcfunction(L, &GctkLuaFnVec3Dot);
	lua_setfield(L, -2, "dot");
	lua_pushcfunction(L, &GctkLuaFnVec3Dist);
	lua_setfield(L, -2, "distance");
	lua_pushcfunction(L, &GctkLuaFnVec3Norm);
	lua_setfield(L, -2, "normalize");
	lua_pushcfunction(L, &GctkLuaFnVec3Cross);
	lua_setfield(L, -2, "cross");
	lua_pushcfunction(L, &GctkLuaFnVec3XY);
	lua_setfield(L, -2, "xy");
	lua_pushcfunction(L, &GctkLuaFnVec3Items);
	lua_setfield(L, -2, "items");

	lua_getglobal(L, "Vec3Meta");
	lua_setmetatable(L, -2);
}

void GctkLuaRegisterVec3(lua_State* L) {
	lua_newtable(L);
	lua_newtable(L);

	lua_pushcfunction(L, &GctkLuaFnVec3Add);
	lua_setfield(L, -2, "__add");
	lua_pushcfunction(L, &GctkLuaFnVec3Sub);
	lua_setfield(L, -2, "__sub");
	lua_pushcfunction(L, &GctkLuaFnVec3Mul);
	lua_setfield(L, -2, "__mul");
	lua_pushcfunction(L, &GctkLuaFnVec3Div);
	lua_setfield(L, -2, "__div");
	lua_pushcfunction(L, &GctkLuaFnVec3Eq);
	lua_setfield(L, -2, "__eq");
	lua_pushcfunction(L, &GctkLuaFnVec3ToStr);
	lua_setfield(L, -2, "__tostring");
	lua_pushcfunction(L, &GctkLuaFnVec3Len);
	lua_setfield(L, -2, "__len");
	lua_pushcfunction(L, &GctkLuaFnVec3Neg);
	lua_setfield(L, -2, "__unm");
	lua_pushstring(L, "Vec3");
	lua_setfield(L, -2, "__name");
	lua_pushstring(L, "Vec3");
	lua_setfield(L, -2, "__classname");

	lua_setglobal(L, "Vec3Meta");

	lua_pushcfunction(L, &GctkLuaFnVec3New);
	lua_setfield(L, -2, "New");
	lua_pushcfunction(L, &GctkLuaFnVec3FromVec2);
	lua_setfield(L, -2, "FromVec2");
	lua_pushcfunction(L, &GctkLuaFnVec3UnitX);
	lua_setfield(L, -2, "UnitX");
	lua_pushcfunction(L, &GctkLuaFnVec3UnitY);
	lua_setfield(L, -2, "UnitY");
	lua_pushcfunction(L, &GctkLuaFnVec3UnitZ);
	lua_setfield(L, -2, "UnitZ");
	lua_pushcfunction(L, &GctkLuaFnVec3Zero);
	lua_setfield(L, -2, "Zero");
	lua_pushcfunction(L, &GctkLuaFnVec3One);
	lua_setfield(L, -2, "One");
	lua_pushcfunction(L, &GctkLuaFnVec3Random);
	lua_setfield(L, -2, "Random");
	lua_pushcfunction(L, &GctkLuaFnVec3Lerp);
	lua_setfield(L, -2, "Lerp");
	lua_setglobal(L, "Vec3");
}

static int GctkLuaFnVec3New(lua_State* L) {
	lua_Number x = 0, y = 0, z = 0;

	if (lua_gettop(L) == 1) {
		x = y = z = luaL_checknumber(L, 1);
	} else if (lua_gettop(L) >= 3) {
		x = luaL_checknumber(L, 1);
		y = luaL_checknumber(L, 2);
		z = luaL_checknumber(L, 3);
	} else {
		luaL_error(L, "Too few arguments! Expected 0, 1 or 3!");
	}

	GctkLuaPushVec3(L, Vec3Create(x, y, z));
	return 1;
}
static int GctkLuaFnVec3FromVec2(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	Vec2 v = GctkLuaGetVec2(L, 1);

	if (lua_gettop(L) == 1) {
		GctkLuaPushVec3(L, Vec3Create(v.x, v.y, 0));
	} else {
		GctkLuaPushVec3(L, Vec3Create(v.x, v.y, luaL_checknumber(L, 2)));
	}

	return 1;
}

static int GctkLuaFnVec3UnitX(lua_State* L) {
	GctkLuaPushVec3(L, VEC3_UNIT_X);
	return 1;
}
static int GctkLuaFnVec3UnitY(lua_State* L) {
	GctkLuaPushVec3(L, VEC3_UNIT_Y);
	return 1;
}
static int GctkLuaFnVec3UnitZ(lua_State* L) {
	GctkLuaPushVec3(L, VEC3_UNIT_Z);
	return 1;
}
static int GctkLuaFnVec3Zero(lua_State* L) {
	GctkLuaPushVec3(L, VEC3_ZERO);
	return 1;
}
static int GctkLuaFnVec3One(lua_State* L) {
	GctkLuaPushVec3(L, VEC3_ONE);
	return 1;
}
static int GctkLuaFnVec3Random(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	GctkLuaPushVec3(L, RandomVec3(GctkLuaGetVec3(L, 1), GctkLuaGetVec3(L, 2)));
	return 1;
}
static int GctkLuaFnVec3Lerp(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	GctkLuaPushVec3(L, Vec3Lerp(GctkLuaGetVec3(L, 1), GctkLuaGetVec3(L, 2), (float)luaL_checknumber(L, 3)));
	return 1;
}

static int GctkLuaFnVec3Add(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	GctkLuaPushVec3(L, Vec3Add(GctkLuaGetVec3(L, 1), GctkLuaGetVec3(L, 2)));
	return 1;
}
static int GctkLuaFnVec3Sub(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	GctkLuaPushVec3(L, Vec3Sub(GctkLuaGetVec3(L, 1), GctkLuaGetVec3(L, 2)));
	return 1;
}
static int GctkLuaFnVec3Mul(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	if (lua_istable(L, 1)) {
		GctkLuaPushVec3(L, Vec3Mul(GctkLuaGetVec3(L, 1), GctkLuaGetVec3(L, 2)));
	} else {
		GctkLuaPushVec3(L, Vec3MulScalar(GctkLuaGetVec3(L, 1), (float)luaL_checknumber(L, 2)));
	}
	return 1;
}
static int GctkLuaFnVec3Div(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	if (lua_istable(L, 1)) {
		GctkLuaPushVec3(L, Vec3Div(GctkLuaGetVec3(L, 1), GctkLuaGetVec3(L, 2)));
	} else {
		GctkLuaPushVec3(L, Vec3DivScalar(GctkLuaGetVec3(L, 1), (float)luaL_checknumber(L, 2)));
	}
	return 1;
}
static int GctkLuaFnVec3Eq(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	lua_pushboolean(L, Vec3Eq(GctkLuaGetVec3(L, 1), GctkLuaGetVec3(L, 2)));
	return 1;
}
static int GctkLuaFnVec3Neg(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	GctkLuaPushVec3(L, Vec3Neg(GctkLuaGetVec3(L, 1)));
	return 1;
}
static int GctkLuaFnVec3ToStr(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	Vec3 v = GctkLuaGetVec3(L, 1);
	char result[128] = { 0 };
	snprintf(result, 64, "%f, %f, %f", v.x, v.y, v.z);
	lua_pushstring(L, result);

	return 1;
}

static int GctkLuaFnVec3Len(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_pushnumber(L, Vec3Length(GctkLuaGetVec3(L, 1)));
	return 1;
}
static int GctkLuaFnVec3Dot(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	lua_pushnumber(L, Vec3Dot(GctkLuaGetVec3(L, 1), GctkLuaGetVec3(L, 2)));
	return 1;
}
static int GctkLuaFnVec3Dist(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	lua_pushnumber(L, Vec3Dist(GctkLuaGetVec3(L, 1), GctkLuaGetVec3(L, 2)));
	return 1;
}
static int GctkLuaFnVec3Cross(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	GctkLuaPushVec3(L, Vec3Cross(GctkLuaGetVec3(L, 1), GctkLuaGetVec3(L, 2)));
	return 1;
}
static int GctkLuaFnVec3XY(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	Vec3 v = GctkLuaGetVec3(L, 1);
	GctkLuaPushVec2(L, Vec2Create(v.x, v.y));
	return 1;
}
static int GctkLuaFnVec3Norm(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	GctkLuaPushVec3(L, Vec3Norm(GctkLuaGetVec3(L, 1)));
	return 1;
}
static int GctkLuaFnVec3Items(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	Vec3 v = GctkLuaGetVec3(L, 1);
	lua_pushnumber(L, v.x);
	lua_pushnumber(L, v.y);
	lua_pushnumber(L, v.z);

	return 3;
}