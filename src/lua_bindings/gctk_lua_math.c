#include "gctk_common.h"
#include "gctk_math.h"
#include "gctk_lua.h"

GCTK_API void GctkLuaRegisterMath(lua_State* L);

static int GctkLuaFnRound(lua_State* L);
static int GctkLuaFnLog2(lua_State* L);
static int GctkLuaFnClamp(lua_State* L);
static int GctkLuaFnLerp(lua_State* L);
static int GctkLuaFnRandom(lua_State* L);

void GctkLuaRegisterMath(lua_State* L) {
	lua_newtable(L);
	// Aliases
	lua_getglobal(L, "math");
	lua_getfield(L, -1, "abs");
	lua_setfield(L, -3, "Abs");
	lua_getfield(L, -1, "pi");
	lua_setfield(L, -3, "Pi");

	lua_getfield(L, -1, "sin");
	lua_setfield(L, -3, "Sin");
	lua_getfield(L, -1, "sinh");
	lua_setfield(L, -3, "Sinh");
	lua_getfield(L, -1, "cos");
	lua_setfield(L, -3, "Cos");
	lua_getfield(L, -1, "cosh");
	lua_setfield(L, -3, "Cosh");
	lua_getfield(L, -1, "tan");
	lua_setfield(L, -3, "Tan");
	lua_getfield(L, -1, "asin");
	lua_setfield(L, -3, "Asin");
	lua_getfield(L, -1, "acos");
	lua_setfield(L, -3, "Acos");
	lua_getfield(L, -1, "atan");
	lua_setfield(L, -3, "Atan");
	lua_getfield(L, -1, "atan2");
	lua_setfield(L, -3, "Atan2");

	lua_getfield(L, -1, "pow");
	lua_setfield(L, -3, "Pow");
	lua_getfield(L, -1, "exp");
	lua_setfield(L, -3, "Exp");
	lua_getfield(L, -1, "sqrt");
	lua_setfield(L, -3, "Sqrt");
	lua_getfield(L, -1, "log");
	lua_setfield(L, -3, "Log");
	lua_getfield(L, -1, "log10");
	lua_setfield(L, -3, "Log10");

	lua_getfield(L, -1, "max");
	lua_setfield(L, -3, "Max");
	lua_getfield(L, -1, "min");
	lua_setfield(L, -3, "Min");

	lua_getfield(L, -1, "floor");
	lua_setfield(L, -3, "Floor");
	lua_getfield(L, -1, "ceil");
	lua_setfield(L, -3, "Ceil");

	lua_getfield(L, -1, "fmod");
	lua_setfield(L, -3, "FMod");
	lua_getfield(L, -1, "modf");
	lua_setfield(L, -3, "ModF");

	lua_getfield(L, -1, "deg");
	lua_setfield(L, -3, "RadToDeg");
	lua_getfield(L, -1, "rad");
	lua_setfield(L, -3, "DegToRad");
	lua_pop(L, 1);

	// New functions
	lua_pushcfunction(L, &GctkLuaFnLerp);
	lua_setfield(L, -2, "Lerp");
	lua_pushcfunction(L, &GctkLuaFnClamp);
	lua_setfield(L, -2, "Clamp");
	lua_pushcfunction(L, &GctkLuaFnRound);
	lua_setfield(L, -2, "Round");
	lua_pushcfunction(L, &GctkLuaFnLog2);
	lua_setfield(L, -2, "Log2");
	lua_setglobal(L, "Math");

	lua_newtable(L);
	lua_getglobal(L, "math");
	lua_getfield(L, -1, "randomseed");
	lua_setfield(L, -3, "Seed");
	lua_pop(L, 1);
	lua_pushcfunction(L, &GctkLuaFnRandom);
	lua_setfield(L, -2, "Next");
	lua_setglobal(L, "Random");
}

static int GctkLuaFnLerp(lua_State* L) {
	if (lua_gettop(L) < 3){
		luaL_error(L, "Expected 3 arguments!");
		return 0;
	}

	lua_pushnumber(L, LerpD(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3)));
	return 1;
}
static int GctkLuaFnClamp(lua_State* L) {
	if (lua_gettop(L) < 3){
		luaL_error(L, "Expected 3 arguments!");
		return 0;
	}

	lua_pushnumber(L, ClampD(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3)));
	return 1;
}
static int GctkLuaFnRound(lua_State* L) {
	if (lua_gettop(L) < 1){
		luaL_error(L, "Expected an argument!");
		return 0;
	}

	lua_pushnumber(L, round(luaL_checknumber(L, 1)));
	return 1;
}
static int GctkLuaFnLog2(lua_State* L) {
	if (lua_gettop(L) < 1){
		luaL_error(L, "Expected an argument!");
		return 0;
	}

	lua_pushnumber(L, log2(luaL_checknumber(L, 1)));
	return 1;
}
static int GctkLuaFnRandom(lua_State* L) {
	if (lua_isnumber(L, 1) && lua_isnumber(L, 2)) {
		lua_pushvalue(L, 1);
		lua_pushvalue(L, 2);
		lua_getglobal(L, "math");
		lua_call(L, 2, 1);
	}
	return 1;
}