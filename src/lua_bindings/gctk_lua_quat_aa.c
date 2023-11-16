#include "gctk_common.h"

#include "gctk_lua.h"
#include "gctk_math.h"

static int GctkLuaFnQuatNew(lua_State* L);
static int GctkLuaFnQuatUnitX(lua_State* L);
static int GctkLuaFnQuatUnitY(lua_State* L);
static int GctkLuaFnQuatUnitZ(lua_State* L);
static int GctkLuaFnQuatUnitW(lua_State* L);
static int GctkLuaFnQuatZero(lua_State* L);
static int GctkLuaFnQuatOne(lua_State* L);
static int GctkLuaFnQuatFromVec4(lua_State* L);
static int GctkLuaFnQuatFromAxisAngle(lua_State* L);

static int GctkLuaFnQuatAdd(lua_State* L);
static int GctkLuaFnQuatSub(lua_State* L);
static int GctkLuaFnQuatMul(lua_State* L);
static int GctkLuaFnQuatDiv(lua_State* L);
static int GctkLuaFnQuatEq(lua_State* L);
static int GctkLuaFnQuatNeg(lua_State* L);
static int GctkLuaFnQuatToStr(lua_State* L);

static int GctkLuaFnQuatLen(lua_State* L);
static int GctkLuaFnQuatDot(lua_State* L);
static int GctkLuaFnQuatDist(lua_State* L);
static int GctkLuaFnQuatNorm(lua_State* L);
static int GctkLuaFnQuatToVec4(lua_State* L);
static int GctkLuaFnQuatToAxisAngle(lua_State* L);
static int GctkLuaFnQuatItems(lua_State* L);

static int GctkLuaFnAxisAngleNew(lua_State* L);
static int GctkLuaFnAxisAngleFromQuat(lua_State* L);
static int GctkLuaFnAxisAngleFromVec4(lua_State* L);

static int GctkLuaFnAxisAngleEq(lua_State* L);
static int GctkLuaFnAxisAngleToVec4(lua_State* L);
static int GctkLuaFnAxisAngleToQuat(lua_State* L);
static int GctkLuaFnAxisAngleItems(lua_State* L);

GCTK_API AxisAngle GctkLuaGetAxisAngle(lua_State* L, int idx);
GCTK_API void GctkLuaPushAxisAngle(lua_State* L, AxisAngle aa);

GCTK_API void GctkLuaRegisterQuat(lua_State* L);
GCTK_API Quat GctkLuaGetQuat(lua_State* L, int idx);
GCTK_API void GctkLuaPushQuat(lua_State* L, Quat vec);

extern Vec2 GctkLuaGetVec2(lua_State* L, int idx);
extern void GctkLuaPushVec2(lua_State* L, Vec2 vec);
extern Vec3 GctkLuaGetVec3(lua_State* L, int idx);
extern void GctkLuaPushVec3(lua_State* L, Vec3 vec);
extern Vec4 GctkLuaGetVec4(lua_State* L, int idx);
extern void GctkLuaPushVec4(lua_State* L, Vec4 vec);

AxisAngle GctkLuaGetAxisAngle(lua_State* L, int idx) {
	lua_getfield(L, idx, "axis");
	Vec3 axis = GctkLuaGetVec3(L, -1);
	lua_pop(L, -1);
	lua_getfield(L, idx, "angle");
	float angle = (float) lua_tonumber(L, -1);
	return (AxisAngle){ .axis = axis, .angle = angle };
}
void GctkLuaPushAxisAngle(lua_State* L, AxisAngle aa) {
	lua_newtable(L);
	GctkLuaPushVec3(L, aa.axis);
	lua_setfield(L, -2, "axis");
	lua_pushnumber(L, aa.angle);
	lua_setfield(L, -2, "angle");
	lua_pushcfunction(L, &GctkLuaFnAxisAngleToVec4);
	lua_setfield(L, -2, "toVec4");
	lua_pushcfunction(L, &GctkLuaFnAxisAngleToQuat);
	lua_setfield(L, -2, "toQuat");
	lua_pushcfunction(L, &GctkLuaFnAxisAngleItems);
	lua_setfield(L, -2, "items");

	lua_getglobal(L, "AxisAngleMeta");
	lua_setmetatable(L, -2);
}

Quat GctkLuaGetQuat(lua_State* L, int idx) {
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
	lua_pop(L, 1);

	return Vec4Create(x, y, z, w);
}
void GctkLuaPushQuat(lua_State* L, Quat vec) {
	lua_newtable(L);
	lua_pushnumber(L, vec.x);
	lua_setfield(L, -2, "x");
	lua_pushnumber(L, vec.y);
	lua_setfield(L, -2, "y");
	lua_pushnumber(L, vec.z);
	lua_setfield(L, -2, "z");
	lua_pushnumber(L, vec.w);
	lua_setfield(L, -2, "w");

	lua_pushcfunction(L, &GctkLuaFnQuatLen);
	lua_setfield(L, -2, "length");
	lua_pushcfunction(L, &GctkLuaFnQuatDot);
	lua_setfield(L, -2, "dot");
	lua_pushcfunction(L, &GctkLuaFnQuatDist);
	lua_setfield(L, -2, "distance");
	lua_pushcfunction(L, &GctkLuaFnQuatNorm);
	lua_setfield(L, -2, "normalize");
	lua_pushcfunction(L, &GctkLuaFnQuatToAxisAngle);
	lua_setfield(L, -2, "toAxisAngle");
	// Quat and Vec4 are the same in C, but different in Lua, conversion is needed
	lua_pushcfunction(L, &GctkLuaFnQuatToVec4);
	lua_setfield(L, -2, "toVec4");
	lua_pushcfunction(L, &GctkLuaFnQuatItems);
	lua_setfield(L, -2, "items");

	lua_getglobal(L, "QuatMeta");
	lua_setmetatable(L, -2);
}

void GctkLuaRegisterQuat(lua_State* L) {
	lua_newtable(L);
	lua_newtable(L);

	lua_pushcfunction(L, &GctkLuaFnQuatAdd);
	lua_setfield(L, -2, "__add");
	lua_pushcfunction(L, &GctkLuaFnQuatSub);
	lua_setfield(L, -2, "__sub");
	lua_pushcfunction(L, &GctkLuaFnQuatMul);
	lua_setfield(L, -2, "__mul");
	lua_pushcfunction(L, &GctkLuaFnQuatDiv);
	lua_setfield(L, -2, "__div");
	lua_pushcfunction(L, &GctkLuaFnQuatEq);
	lua_setfield(L, -2, "__eq");
	lua_pushcfunction(L, &GctkLuaFnQuatToStr);
	lua_setfield(L, -2, "__tostring");
	lua_pushcfunction(L, &GctkLuaFnQuatLen);
	lua_setfield(L, -2, "__len");
	lua_pushcfunction(L, &GctkLuaFnQuatNeg);
	lua_setfield(L, -2, "__unm");
	lua_pushstring(L, "Quat");
	lua_setfield(L, -2, "__name");
	lua_pushstring(L, "Quat");
	lua_setfield(L, -2, "__classname");

	lua_setglobal(L, "QuatMeta");

	lua_pushcfunction(L, &GctkLuaFnQuatNew);
	lua_setfield(L, -2, "New");
	lua_pushcfunction(L, &GctkLuaFnQuatUnitX);
	lua_setfield(L, -2, "UnitX");
	lua_pushcfunction(L, &GctkLuaFnQuatUnitY);
	lua_setfield(L, -2, "UnitY");
	lua_pushcfunction(L, &GctkLuaFnQuatUnitZ);
	lua_setfield(L, -2, "UnitZ");
	lua_pushcfunction(L, &GctkLuaFnQuatUnitW);
	lua_setfield(L, -2, "UnitW");
	lua_pushcfunction(L, &GctkLuaFnQuatUnitW);
	lua_setfield(L, -2, "Identity");
	lua_pushcfunction(L, &GctkLuaFnQuatZero);
	lua_setfield(L, -2, "Zero");
	lua_pushcfunction(L, &GctkLuaFnQuatOne);
	lua_setfield(L, -2, "One");
	lua_pushcfunction(L, &GctkLuaFnQuatFromAxisAngle);
	lua_setfield(L, -2, "FromAxisAngle");
	// Quat and Vec4 are the same in C, but different in Lua, conversion is needed
	lua_pushcfunction(L, &GctkLuaFnQuatFromVec4);
	lua_setfield(L, -2, "FromVec4");
	lua_setglobal(L, "Quat");

	lua_newtable(L);
	lua_newtable(L);
	lua_pushcfunction(L, &GctkLuaFnAxisAngleEq);
	lua_setfield(L, -2, "__eq");
	lua_pushstring(L, "AxisAngle");
	lua_setfield(L, -2, "__classname");

	lua_setglobal(L, "AxisAngleMeta");

	lua_pushcfunction(L, &GctkLuaFnAxisAngleNew);
	lua_setfield(L, -2, "New");
	lua_pushcfunction(L, &GctkLuaFnAxisAngleFromVec4);
	lua_setfield(L, -2, "FromVec4");
	lua_pushcfunction(L, &GctkLuaFnAxisAngleFromQuat);
	lua_setfield(L, -2, "FromQuat");

	lua_setglobal(L, "AxisAngle");
}

static int GctkLuaFnQuatNew(lua_State* L) {
	lua_Number x = 0, y = 0, z = 0, w = 1;

	if (lua_gettop(L) == 1) {
		x = y = z = w = luaL_checknumber(L, 1);
	} else if (lua_gettop(L) >= 4) {
		x = luaL_checknumber(L, 1);
		y = luaL_checknumber(L, 2);
		z = luaL_checknumber(L, 3);
		w = luaL_checknumber(L, 4);
	} else {
		luaL_error(L, "Too few arguments! Expected 0, 1 or 4!");
	}

	GctkLuaPushQuat(L, QuatCreate(x, y, z, w));
	return 1;
}

static int GctkLuaFnQuatUnitX(lua_State* L) {
	GctkLuaPushQuat(L, QUAT_UNIT_X);
	return 1;
}
static int GctkLuaFnQuatUnitY(lua_State* L) {
	GctkLuaPushQuat(L, QUAT_UNIT_Y);
	return 1;
}
static int GctkLuaFnQuatUnitZ(lua_State* L) {
	GctkLuaPushQuat(L, QUAT_UNIT_Z);
	return 1;
}
static int GctkLuaFnQuatUnitW(lua_State* L) {
	GctkLuaPushQuat(L, QUAT_UNIT_W);
	return 1;
}
static int GctkLuaFnQuatZero(lua_State* L) {
	GctkLuaPushQuat(L, QUAT_ZERO);
	return 1;
}
static int GctkLuaFnQuatOne(lua_State* L) {
	GctkLuaPushQuat(L, QUAT_ONE);
	return 1;
}
static int GctkLuaFnQuatFromVec4(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	GctkLuaPushVec4(L, (Vec4)GctkLuaGetQuat(L, 1));
	return 1;
}
static int GctkLuaFnQuatFromAxisAngle(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	GctkLuaPushQuat(L, QuatFromAxisAngle(GctkLuaGetAxisAngle(L, 1)));
	return 1;
}

static int GctkLuaFnQuatAdd(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	GctkLuaPushQuat(L, QuatAdd(GctkLuaGetQuat(L, 1), GctkLuaGetQuat(L, 2)));
	return 1;
}
static int GctkLuaFnQuatSub(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	GctkLuaPushQuat(L, QuatSub(GctkLuaGetQuat(L, 1), GctkLuaGetQuat(L, 2)));
	return 1;
}
static int GctkLuaFnQuatMul(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	if (lua_istable(L, 1)) {
		GctkLuaPushQuat(L, QuatMul(GctkLuaGetQuat(L, 1), GctkLuaGetQuat(L, 2)));
	} else {
		GctkLuaPushQuat(L, QuatMulScalar(GctkLuaGetQuat(L, 1), (float)luaL_checknumber(L, 2)));
	}
	return 1;
}
static int GctkLuaFnQuatDiv(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	if (lua_istable(L, 1)) {
		GctkLuaPushQuat(L, QuatDiv(GctkLuaGetQuat(L, 1), GctkLuaGetQuat(L, 2)));
	} else {
		GctkLuaPushQuat(L, QuatDivScalar(GctkLuaGetQuat(L, 1), (float)luaL_checknumber(L, 2)));
	}
	return 1;
}
static int GctkLuaFnQuatEq(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	lua_pushboolean(L, QuatEq(GctkLuaGetQuat(L, 1), GctkLuaGetQuat(L, 2)));
	return 1;
}
static int GctkLuaFnQuatNeg(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	GctkLuaPushQuat(L, QuatNeg(GctkLuaGetQuat(L, 1)));
	return 1;
}
static int GctkLuaFnQuatToStr(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	Quat v = GctkLuaGetQuat(L, 1);
	char result[192] = { 0 };
	snprintf(result, 64, "%f, %f, %f, %f", v.x, v.y, v.z, v.w);
	lua_pushstring(L, result);

	return 1;
}

static int GctkLuaFnQuatLen(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_pushnumber(L, QuatLength(GctkLuaGetQuat(L, 1)));
	return 1;
}
static int GctkLuaFnQuatDot(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	lua_pushnumber(L, QuatDot(GctkLuaGetQuat(L, 1), GctkLuaGetQuat(L, 2)));
	return 1;
}
static int GctkLuaFnQuatDist(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	lua_pushnumber(L, QuatDist(GctkLuaGetQuat(L, 1), GctkLuaGetQuat(L, 2)));
	return 1;
}
static int GctkLuaFnQuatNorm(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	GctkLuaPushQuat(L, QuatNorm(GctkLuaGetQuat(L, 1)));
	return 1;
}
static int GctkLuaFnQuatToVec4(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	GctkLuaPushVec4(L, (Vec4)GctkLuaGetQuat(L, 1));
	return 1;
}
static int GctkLuaFnQuatToAxisAngle(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	GctkLuaPushAxisAngle(L, AxisAngleFromQuat(GctkLuaGetQuat(L, 1)));
	return 1;
}
static int GctkLuaFnQuatItems(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	Quat q = GctkLuaGetQuat(L, 1);
	lua_pushnumber(L, q.x);
	lua_pushnumber(L, q.y);
	lua_pushnumber(L, q.z);
	lua_pushnumber(L, q.w);

	return 4;
}

static int GctkLuaFnAxisAngleNew(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	Vec3 axis = GctkLuaGetVec3(L, 1);
	float angle = (float)luaL_checknumber(L, 2);

	GctkLuaPushAxisAngle(L, (AxisAngle){ .axis = axis, .angle = angle });
	return 1;
}
static int GctkLuaFnAxisAngleFromQuat(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	GctkLuaPushAxisAngle(L, AxisAngleFromQuat(GctkLuaGetQuat(L, 1)));
	return 1;
}
static int GctkLuaFnAxisAngleFromVec4(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	GctkLuaPushAxisAngle(L, AxisAngleFromVec4(GctkLuaGetVec4(L, 1)));
	return 1;
}

static int GctkLuaFnAxisAngleEq(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	AxisAngle a = GctkLuaGetAxisAngle(L, 1);
	AxisAngle b = GctkLuaGetAxisAngle(L, 2);
	lua_pushboolean(L, Vec4Eq(a.axis_angle, b.axis_angle));
	return 1;
}
static int GctkLuaFnAxisAngleToVec4(lua_State* L) {
	GctkLuaPushVec4(L, GctkLuaGetAxisAngle(L, 1).axis_angle);
	return 1;
}
static int GctkLuaFnAxisAngleToQuat(lua_State* L) {
	GctkLuaPushQuat(L, QuatFromAxisAngle(GctkLuaGetAxisAngle(L, 1)));
	return 1;
}
static int GctkLuaFnAxisAngleItems(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	AxisAngle aa = GctkLuaGetAxisAngle(L, 1);
	GctkLuaPushVec3(L, aa.axis);
	lua_pushnumber(L, aa.angle);

	return 2;
}