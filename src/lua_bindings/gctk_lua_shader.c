#include "gctk_common.h"
#include "gctk_lua.h"
#include "gctk_shader.h"
#include "gctk_string.h"

GCTK_API void GctkLuaRegisterShader(lua_State* L);
GCTK_API void GctkLuaPushShader(lua_State* L, const Shader* shader);
GCTK_API Shader* GctkLuaGetShader(lua_State* L, int idx);

extern Vec2 GctkLuaGetVec2(lua_State* L, int idx);
extern void GctkLuaPushVec2(lua_State* L, Vec2 vec);
extern Vec3 GctkLuaGetVec3(lua_State* L, int idx);
extern void GctkLuaPushVec3(lua_State* L, Vec3 vec);
extern Quat GctkLuaGetQuat(lua_State* L, int idx);
extern void GctkLuaPushQuat(lua_State* L, Quat quat);
extern Color GctkLuaGetColor(lua_State* L, int idx);
extern void GctkLuaPushColor(lua_State* L, Color quat);
extern Vec4 GctkLuaGetVec4(lua_State* L, int idx);
extern void GctkLuaPushVec4(lua_State* L, Vec4 vec);
extern Mat4 GctkLuaGetMat4(lua_State* L, int idx);
extern void GctkLuaPushMat4(lua_State* L, Mat4 mat);

static int GctkLuaFnShaderCompile(lua_State* L);

static int GctkLuaFnShaderApply(lua_State* L);
static int GctkLuaFnShaderSetUniform(lua_State* L);
static int GctkLuaFnShaderGetUniform(lua_State* L);

static int GctkLuaFnShaderEq(lua_State* L);
static int GctkLuaFnShaderDrop(lua_State* L);
static int GctkLuaFnShaderToStr(lua_State* L);

void GctkLuaRegisterShader(lua_State* L) {
	lua_newtable(L);
	lua_newtable(L);

	lua_pushcfunction(L, &GctkLuaFnShaderEq);
	lua_setfield(L, -2, "__eq");
	lua_pushcfunction(L, &GctkLuaFnShaderDrop);
	lua_setfield(L, -2, "__gc");
	lua_pushcfunction(L, &GctkLuaFnShaderToStr);
	lua_setfield(L, -2, "__tostring");
	lua_pushstring(L, "Shader");
	lua_setfield(L, -2, "__name");
	lua_pushstring(L, "Shader");
	lua_setfield(L, -2, "__classname");
	lua_setglobal(L, "ShaderMeta");

	lua_pushcfunction(L, &GctkLuaFnShaderCompile);
	lua_setfield(L, -2, "Compile");
	lua_setglobal(L, "Shader");
}

void GctkLuaPushShader(lua_State* L, const Shader* shader) {
	Shader* p = (Shader*)lua_newuserdata(L, sizeof(Shader));
	memcpy(p, shader, sizeof(Shader));
	lua_pushcfunction(L, &GctkLuaFnShaderApply);
	lua_setfield(L, -2, "apply");
	lua_pushcfunction(L, &GctkLuaFnShaderSetUniform);
	lua_setfield(L, -2, "uniform");
	lua_pushcfunction(L, &GctkLuaFnShaderGetUniform);
	lua_setfield(L, -2, "getUniform");
}
Shader* GctkLuaGetShader(lua_State* L, int idx) {
	return (Shader*)luaL_checkudata(L, idx, "Shader");
}

static int GctkLuaFnShaderCompile(lua_State* L) {
	Shader shader;
	if (lua_gettop(L) == 1) {
		GctkCompileComputeShader(luaL_checkstring(L, 1), &shader);
	} else {
		GctkCompileShader(luaL_checkstring(L, 1), luaL_checkstring(L, 2), &shader);
	}
	GctkLuaPushShader(L, &shader);
	return 1;
}

static int GctkLuaFnShaderApply(lua_State* L) {
	GctkUseShader((Shader*)luaL_checkudata(L, 1, "Shader"));
	return 1;
}
static int GctkLuaFnShaderSetUniform(lua_State* L) {
	if (lua_gettop(L) < 2) {
		luaL_error(L, "Expected 2 arguments!");
		return 0;
	}

	Shader* shader = (Shader*)luaL_checkudata(L, 1, "Shader");
	const char* name = lua_tostring(L, 1);

	if (lua_isboolean(L, 2)) {
		GctkShaderSetUniformUInt(shader, name, lua_toboolean(L, 2));
	} else if (lua_isinteger(L, 2)) {
		GctkShaderSetUniformInt(shader, name, lua_tointeger(L, 2));
	} else if (lua_istable(L, 2)) {
		if (lua_getmetatable(L, -1)) {
			lua_getfield(L, -1, "__name");
			const char* type = lua_tostring(L, -1);
			if (strcmp(type, "Vec2") == 0) {
				GctkShaderSetUniformVec2(shader, name, GctkLuaGetVec2(L, 2));
			} else if (strcmp(type, "Vec3") == 0) {
				GctkShaderSetUniformVec3(shader, name, GctkLuaGetVec3(L, 2));
			} else if (strcmp(type, "Vec4") == 0) {
				GctkShaderSetUniformVec4(shader, name, GctkLuaGetVec4(L, 2));
			} else if (strcmp(type, "Quat") == 0) {
				GctkShaderSetUniformQuat(shader, name, GctkLuaGetQuat(L, 2));
			} else if (strcmp(type, "Color") == 0) {
				GctkShaderSetUniformColor(shader, name, GctkLuaGetColor(L, 2));
			} else if (strcmp(type, "Mat4") == 0) {
				GctkShaderSetUniformMat4(shader, name, GctkLuaGetMat4(L, 2));
			} else {
				// TODO: Add support for tables
				luaL_error(L, "Could not set uniform \"%s\". Unsupported table type \"%s\"!", name, type);
				return 0;
			}
			lua_pop(L, 2);
		} else {
			// TODO: Add support for tables
			luaL_error(L, "Could not set uniform \"%s\". Unsupported type \"%s\"!", name, lua_typename(L, 2));
			return 0;
		}
	} else if (lua_isuserdata(L, 2)) {
		if (lua_getmetatable(L, -1)) {
			lua_getfield(L, -1, "__name");
			const char* type = lua_tostring(L, -1);
			if (strcmp(type, "Texture") == 0) {
				Texture* texture = (Texture*)luaL_checkudata(L, 2, "Texture");
				if (texture) {
					GctkShaderSetUniformTexture(shader, name, texture);
				} else {
					GctkShaderSetUniformUInt(shader, name, 0);
				}
			}
			lua_pop(L, 2);
		} else {
			luaL_error(L, "Could not set uniform \"%s\". Unsupported type \"%s\"!", name, lua_typename(L, 2));
			return 0;
		}
	} else {
		luaL_error(L, "Could not set uniform \"%s\". Unsupported type \"%s\"!", name, lua_typename(L, 2));
		return 0;
	}

	return 1;
}
static int GctkLuaFnShaderGetUniform(lua_State* L) {
	Shader* shader = (Shader*)luaL_checkudata(L, 1, "Shader");
	const char* name = lua_tostring(L, 2);
	const char* type = lua_tostring(L, 3);

	if (GctkStrEq(type, "number", false)) {
		float value;
		GctkShaderGetUniformFloat(shader, name, &value);
		lua_pushnumber(L, value);
	} else if (GctkStrEq(type, "integer", false)) {
		int value;
		GctkShaderGetUniformInt(shader, name, &value);
		lua_pushinteger(L, value);
	} else if (GctkStrEq(type, "boolean", false)) {
		uint32_t value;
		GctkShaderGetUniformUInt(shader, name, &value);
		lua_pushboolean(L, value != 0);
	} else if (GctkStrEq(type, "Vec2", false)) {
		Vec2 value;
		GctkShaderGetUniformVec2(shader, name, &value);
		GctkLuaPushVec2(L, value);
	} else if (GctkStrEq(type, "Vec3", false)) {
		Vec3 value;
		GctkShaderGetUniformVec3(shader, name, &value);
		GctkLuaPushVec3(L, value);
	} else if (GctkStrEq(type, "Vec4", false)) {
		Vec4 value;
		GctkShaderGetUniformVec4(shader, name, &value);
		GctkLuaPushVec4(L, value);
	} else if (GctkStrEq(type, "Quat", false)) {
		Quat value;
		GctkShaderGetUniformQuat(shader, name, &value);
		GctkLuaPushQuat(L, value);
	} else if (GctkStrEq(type, "Color", false)) {
		Color value;
		GctkShaderGetUniformColor(shader, name, &value);
		GctkLuaPushColor(L, value);
	} else if (GctkStrEq(type, "Mat4", false)) {
		Mat4 value;
		GctkShaderGetUniformMat4(shader, name, &value);
		GctkLuaPushMat4(L, value);
	} else {
		luaL_error(L, "Could not set uniform, unsupported/invalid type \"%s\"!", type);
		return 0;
	}

	return 1;
}

static int GctkLuaFnShaderEq(lua_State* L) {
	const Shader* a = (const Shader*)luaL_checkudata(L, 1, "Shader");
	const Shader* b = (const Shader*)luaL_checkudata(L, 1, "Shader");
	lua_pushboolean(L, a && b && a->id == b->id);

	return 1;
}
static int GctkLuaFnShaderDrop(lua_State* L) {
	Shader* shader = (Shader*)luaL_checkudata(L, 1, "Shader");
	GctkFreeShader(shader);
	return 0;
}
static int GctkLuaFnShaderToStr(lua_State* L) {
	const Shader* shader = (const Shader*)luaL_checkudata(L, 1, "Shader");

	char temp[128] = { 0 };
	snprintf(temp, 128, "Shader:%u", shader->id);
	return 1;
}