#include "gctk_common.h"

#include "gctk_lua.h"
#include "gctk_math.h"

static int GctkLuaFnVec4New(lua_State* L);
static int GctkLuaFnVec4UnitX(lua_State* L);
static int GctkLuaFnVec4UnitY(lua_State* L);
static int GctkLuaFnVec4UnitZ(lua_State* L);
static int GctkLuaFnVec4UnitW(lua_State* L);
static int GctkLuaFnVec4Zero(lua_State* L);
static int GctkLuaFnVec4One(lua_State* L);
static int GctkLuaFnVec4FromVec3(lua_State* L);
static int GctkLuaFnVec4Random(lua_State* L);
static int GctkLuaFnVec4Lerp(lua_State* L);

static int GctkLuaFnVec4Add(lua_State* L);
static int GctkLuaFnVec4Sub(lua_State* L);
static int GctkLuaFnVec4Mul(lua_State* L);
static int GctkLuaFnVec4Div(lua_State* L);
static int GctkLuaFnVec4Eq(lua_State* L);
static int GctkLuaFnVec4Neg(lua_State* L);
static int GctkLuaFnVec4ToStr(lua_State* L);

static int GctkLuaFnVec4Len(lua_State* L);
static int GctkLuaFnVec4Dot(lua_State* L);
static int GctkLuaFnVec4Dist(lua_State* L);
static int GctkLuaFnVec4Norm(lua_State* L);
static int GctkLuaFnVec4XY(lua_State* L);
static int GctkLuaFnVec4ZW(lua_State* L);
static int GctkLuaFnVec4XYZ(lua_State* L);
static int GctkLuaFnVec4Items(lua_State* L);

GCTK_API void GctkLuaRegisterVec4(lua_State* L);
GCTK_API Vec4 GctkLuaGetVec4(lua_State* L, int idx);
GCTK_API void GctkLuaPushVec4(lua_State* L, Vec4 vec);

extern Vec2 GctkLuaGetVec2(lua_State* L, int idx);
extern void GctkLuaPushVec2(lua_State* L, Vec2 vec);
extern Vec3 GctkLuaGetVec3(lua_State* L, int idx);
extern void GctkLuaPushVec3(lua_State* L, Vec3 vec);

Vec4 GctkLuaGetVec4(lua_State* L, int idx) {
	lua_getfield(L, idx, "x");
	float x = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);
	lua_getfield(L, idx, "y");
	float y = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);
	lua_getfield(L, idx, "z");
	float z = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);
	lua_getfield(L, idx, "w");
	float w = (float)lua_tonumber(L, -1);
	lua_pop(L, 2);

	return Vec4Create(x, y, z, w);
}
void GctkLuaPushVec4(lua_State* L, Vec4 vec) {
	lua_newtable(L);
	lua_pushnumber(L, vec.x);
	lua_setfield(L, -2, "x");
	lua_pushnumber(L, vec.y);
	lua_setfield(L, -2, "y");
	lua_pushnumber(L, vec.z);
	lua_setfield(L, -2, "z");
	lua_pushnumber(L, vec.w);
	lua_setfield(L, -2, "w");

	lua_pushcfunction(L, &GctkLuaFnVec4Len);
	lua_setfield(L, -2, "length");
	lua_pushcfunction(L, &GctkLuaFnVec4Dot);
	lua_setfield(L, -2, "dot");
	lua_pushcfunction(L, &GctkLuaFnVec4Dist);
	lua_setfield(L, -2, "distance");
	lua_pushcfunction(L, &GctkLuaFnVec4Norm);
	lua_setfield(L, -2, "normalize");
	lua_pushcfunction(L, &GctkLuaFnVec4XY);
	lua_setfield(L, -2, "xy");
	lua_pushcfunction(L, &GctkLuaFnVec4ZW);
	lua_setfield(L, -2, "zw");
	lua_pushcfunction(L, &GctkLuaFnVec4XYZ);
	lua_setfield(L, -2, "xyz");
	lua_pushcfunction(L, &GctkLuaFnVec4Items);
	lua_setfield(L, -2, "items");

	lua_getglobal(L, "Vec4Meta");
	lua_setmetatable(L, -2);
}

void GctkLuaRegisterVec4(lua_State* L) {
	lua_newtable(L);
	lua_newtable(L);

	lua_pushcfunction(L, &GctkLuaFnVec4Add);
	lua_setfield(L, -2, "__add");
	lua_pushcfunction(L, &GctkLuaFnVec4Sub);
	lua_setfield(L, -2, "__sub");
	lua_pushcfunction(L, &GctkLuaFnVec4Mul);
	lua_setfield(L, -2, "__mul");
	lua_pushcfunction(L, &GctkLuaFnVec4Div);
	lua_setfield(L, -2, "__div");
	lua_pushcfunction(L, &GctkLuaFnVec4Eq);
	lua_setfield(L, -2, "__eq");
	lua_pushcfunction(L, &GctkLuaFnVec4ToStr);
	lua_setfield(L, -2, "__tostring");
	lua_pushcfunction(L, &GctkLuaFnVec4Len);
	lua_setfield(L, -2, "__len");
	lua_pushcfunction(L, &GctkLuaFnVec4Neg);
	lua_setfield(L, -2, "__unm");
	lua_pushstring(L, "Vec4");
	lua_setfield(L, -2, "__name");
	lua_pushstring(L, "Vec4");
	lua_setfield(L, -2, "__classname");

	lua_setglobal(L, "Vec4Meta");

	lua_pushcfunction(L, &GctkLuaFnVec4New);
	lua_setfield(L, -2, "New");
	lua_pushcfunction(L, &GctkLuaFnVec4UnitX);
	lua_setfield(L, -2, "UnitX");
	lua_pushcfunction(L, &GctkLuaFnVec4UnitY);
	lua_setfield(L, -2, "UnitY");
	lua_pushcfunction(L, &GctkLuaFnVec4UnitZ);
	lua_setfield(L, -2, "UnitZ");
	lua_pushcfunction(L, &GctkLuaFnVec4UnitW);
	lua_setfield(L, -2, "UnitW");
	lua_pushcfunction(L, &GctkLuaFnVec4Zero);
	lua_setfield(L, -2, "Zero");
	lua_pushcfunction(L, &GctkLuaFnVec4One);
	lua_setfield(L, -2, "One");
	lua_pushcfunction(L, &GctkLuaFnVec4FromVec3);
	lua_setfield(L, -2, "FromVec3");
	lua_pushcfunction(L, &GctkLuaFnVec4Random);
	lua_setfield(L, -2, "Random");
	lua_pushcfunction(L, &GctkLuaFnVec4Lerp);
	lua_setfield(L, -2, "Lerp");
	lua_setglobal(L, "Vec4");
}

static int GctkLuaFnVec4New(lua_State* L) {
	lua_Number x = 0, y = 0, z = 0, w = 0;

	if (lua_gettop(L) == 1) {
		x = y = z = luaL_checknumber(L, 1);
	} else if (lua_gettop(L) >= 4) {
		x = luaL_checknumber(L, 1);
		y = luaL_checknumber(L, 2);
		z = luaL_checknumber(L, 3);
		w = luaL_checknumber(L, 4);
	} else {
		luaL_error(L, "Too few arguments! Expected 0, 1 or 4!");
	}

	GctkLuaPushVec4(L, Vec4Create(x, y, z, w));
	return 1;
}

static int GctkLuaFnVec4UnitX(lua_State* L) {
	GctkLuaPushVec4(L, VEC4_UNIT_X);
	return 1;
}
static int GctkLuaFnVec4UnitY(lua_State* L) {
	GctkLuaPushVec4(L, VEC4_UNIT_Y);
	return 1;
}
static int GctkLuaFnVec4UnitZ(lua_State* L) {
	GctkLuaPushVec4(L, VEC4_UNIT_Z);
	return 1;
}
static int GctkLuaFnVec4UnitW(lua_State* L) {
	GctkLuaPushVec4(L, VEC4_UNIT_W);
	return 1;
}
static int GctkLuaFnVec4Zero(lua_State* L) {
	GctkLuaPushVec4(L, VEC4_ZERO);
	return 1;
}
static int GctkLuaFnVec4One(lua_State* L) {
	GctkLuaPushVec4(L, VEC4_ONE);
	return 1;
}
static int GctkLuaFnVec4FromVec3(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	Vec3 v = GctkLuaGetVec3(L, 1);

	if (lua_gettop(L) == 1) {
		GctkLuaPushVec4(L, Vec4Create(v.x, v.y, v.z, 0));
	} else {
		GctkLuaPushVec4(L, Vec4Create(v.x, v.y, v.z, luaL_checknumber(L, 2)));
	}

	return 1;
}
static int GctkLuaFnVec4Random(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	GctkLuaPushVec4(L, RandomVec4(GctkLuaGetVec4(L, 1), GctkLuaGetVec4(L, 2)));
	return 1;
}
static int GctkLuaFnVec4Lerp(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	GctkLuaPushVec4(L, Vec4Lerp(GctkLuaGetVec4(L, 1), GctkLuaGetVec4(L, 2), (float)luaL_checknumber(L, 3)));
	return 1;
}
static int GctkLuaFnVec4Add(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	GctkLuaPushVec4(L, Vec4Add(GctkLuaGetVec4(L, 1), GctkLuaGetVec4(L, 2)));
	return 1;
}
static int GctkLuaFnVec4Sub(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	GctkLuaPushVec4(L, Vec4Sub(GctkLuaGetVec4(L, 1), GctkLuaGetVec4(L, 2)));
	return 1;
}
static int GctkLuaFnVec4Mul(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	if (lua_istable(L, 1)) {
		GctkLuaPushVec4(L, Vec4Mul(GctkLuaGetVec4(L, 1), GctkLuaGetVec4(L, 2)));
	} else {
		GctkLuaPushVec4(L, Vec4MulScalar(GctkLuaGetVec4(L, 1), (float)luaL_checknumber(L, 2)));
	}
	return 1;
}
static int GctkLuaFnVec4Div(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	if (lua_istable(L, 1)) {
		GctkLuaPushVec4(L, Vec4Div(GctkLuaGetVec4(L, 1), GctkLuaGetVec4(L, 2)));
	} else {
		GctkLuaPushVec4(L, Vec4DivScalar(GctkLuaGetVec4(L, 1), (float)luaL_checknumber(L, 2)));
	}
	return 1;
}
static int GctkLuaFnVec4Eq(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	lua_pushboolean(L, Vec4Eq(GctkLuaGetVec4(L, 1), GctkLuaGetVec4(L, 2)));
	return 1;
}
static int GctkLuaFnVec4Neg(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	GctkLuaPushVec4(L, Vec4Neg(GctkLuaGetVec4(L, 1)));
	return 1;
}
static int GctkLuaFnVec4ToStr(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	Vec4 v = GctkLuaGetVec4(L, 1);
	char result[192] = { 0 };
	snprintf(result, 64, "%f, %f, %f, %f", v.x, v.y, v.z, v.w);
	lua_pushstring(L, result);

	return 1;
}

static int GctkLuaFnVec4Len(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_pushnumber(L, Vec4Length(GctkLuaGetVec4(L, 1)));
	return 1;
}
static int GctkLuaFnVec4Dot(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	lua_pushnumber(L, Vec4Dot(GctkLuaGetVec4(L, 1), GctkLuaGetVec4(L, 2)));
	return 1;
}
static int GctkLuaFnVec4Dist(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	lua_pushnumber(L, Vec4Dist(GctkLuaGetVec4(L, 1), GctkLuaGetVec4(L, 2)));
	return 1;
}
static int GctkLuaFnVec4Norm(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	GctkLuaPushVec4(L, Vec4Norm(GctkLuaGetVec4(L, 1)));
	return 1;
}
static int GctkLuaFnVec4XY(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	Vec4 v = GctkLuaGetVec4(L, 1);
	GctkLuaPushVec2(L, Vec2Create(v.x, v.y));
	return 1;
}
static int GctkLuaFnVec4ZW(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	Vec4 v = GctkLuaGetVec4(L, 1);
	GctkLuaPushVec2(L, Vec2Create(v.z, v.w));
	return 1;
}
static int GctkLuaFnVec4XYZ(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	Vec4 v = GctkLuaGetVec4(L, 1);
	GctkLuaPushVec3(L, Vec3Create(v.x, v.y, v.z));
	return 1;
}
static int GctkLuaFnVec4Items(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	Vec4 v = GctkLuaGetVec4(L, 1);
	lua_pushnumber(L, v.x);
	lua_pushnumber(L, v.y);
	lua_pushnumber(L, v.z);
	lua_pushnumber(L, v.w);

	return 4;
}