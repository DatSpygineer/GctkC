#include "gctk_lua.h"
#include "gctk_math.h"

GCTK_API void GctkLuaRegisterMat4(lua_State* L);
GCTK_API Mat4 GctkLuaGetMat4(lua_State* L, int idx);
GCTK_API void GctkLuaPushMat4(lua_State* L, Mat4 mat);

extern Vec3 GctkLuaGetVec3(lua_State* L, int idx);
extern void GctkLuaPushVec3(lua_State* L, Vec3 vec);
extern Quat GctkLuaGetQuat(lua_State* L, int idx);
extern void GctkLuaPushQuat(lua_State* L, Quat quat);
extern Vec4 GctkLuaGetVec4(lua_State* L, int idx);
extern void GctkLuaPushVec4(lua_State* L, Vec4 vec);
extern AxisAngle GctkLuaGetAxisAngle(lua_State* L, int idx);
extern void GctkLuaPushAxisAngle(lua_State* L, AxisAngle aa);

static int GctkLuaFnMat4New(lua_State* L);
static int GctkLuaFnMat4Identity(lua_State* L);
static int GctkLuaFnMat4CreateOrtho(lua_State* L);
static int GctkLuaFnMat4CreatePerspective(lua_State* L);
static int GctkLuaFnMat4CreateTransform(lua_State* L);

static int GctkLuaFnMat4Transpose(lua_State* L);
static int GctkLuaFnMat4Column(lua_State* L);
static int GctkLuaFnMat4GetTranslation(lua_State* L);
static int GctkLuaFnMat4GetRotation(lua_State* L);
static int GctkLuaFnMat4GetRotationAxisAngle(lua_State* L);
static int GctkLuaFnMat4GetScale(lua_State* L);
static int GctkLuaFnMat4Translate(lua_State* L);
static int GctkLuaFnMat4Rotate(lua_State* L);
static int GctkLuaFnMat4Scale(lua_State* L);

static int GctkLuaFnMat4Mul(lua_State* L);
static int GctkLuaFnMat4Eq(lua_State* L);

void GctkLuaRegisterMat4(lua_State* L) {
	lua_newtable(L);
	lua_newtable(L);

	lua_pushcfunction(L, &GctkLuaFnMat4Mul);
	lua_setfield(L, -2, "__mul");
	lua_pushcfunction(L, &GctkLuaFnMat4Eq);
	lua_setfield(L, -2, "__eq");
	lua_pushstring(L, "Mat4");
	lua_setfield(L, -2, "__name");
	lua_pushstring(L, "Mat4");
	lua_setfield(L, -2, "__classname");

	lua_setglobal(L, "Mat4Meta");

	lua_pushcfunction(L, &GctkLuaFnMat4New);
	lua_setfield(L, -2, "New");
	lua_pushcfunction(L, &GctkLuaFnMat4Identity);
	lua_setfield(L, -2, "Identity");
	lua_pushcfunction(L, &GctkLuaFnMat4CreateOrtho);
	lua_setfield(L, -2, "CreateOrtho");
	lua_pushcfunction(L, &GctkLuaFnMat4CreatePerspective);
	lua_setfield(L, -2, "CreatePerspective");
	lua_pushcfunction(L, &GctkLuaFnMat4CreateTransform);
	lua_setfield(L, -2, "CreateTransform");
}

Mat4 GctkLuaGetMat4(lua_State* L, int idx) {
	Mat4 mat = { 0 };
	for (int i = 0; i < 4; i++) {
		lua_rawgeti(L, idx, i + 1);
		mat.rows[i] = GctkLuaGetVec4(L, -1);
		lua_pop(L, 1);
	}
	return mat;
}
void GctkLuaPushMat4(lua_State* L, Mat4 mat) {
	lua_createtable(L, 4, 0);
	for (int i = 0; i < 4; i++) {
		GctkLuaPushVec4(L, mat.rows[i]);
		lua_rawseti(L, -2, i + 1);
	}

	lua_pushcfunction(L, &GctkLuaFnMat4Transpose);
	lua_setfield(L, -2, "getTranspose");
	lua_pushcfunction(L, &GctkLuaFnMat4Column);
	lua_setfield(L, -2, "getColumn");
	lua_pushcfunction(L, &GctkLuaFnMat4GetTranslation);
	lua_setfield(L, -2, "getTranslation");
	lua_pushcfunction(L, &GctkLuaFnMat4GetRotation);
	lua_setfield(L, -2, "getRotation");
	lua_pushcfunction(L, &GctkLuaFnMat4GetRotationAxisAngle);
	lua_setfield(L, -2, "getAxisAngle");
	lua_pushcfunction(L, &GctkLuaFnMat4GetScale);
	lua_setfield(L, -2, "getScale");
	lua_pushcfunction(L, &GctkLuaFnMat4Translate);
	lua_setfield(L, -2, "translate");
	lua_pushcfunction(L, &GctkLuaFnMat4Rotate);
	lua_setfield(L, -2, "rotate");
	lua_pushcfunction(L, &GctkLuaFnMat4Scale);
	lua_setfield(L, -2, "scale");

	lua_getglobal(L, "Mat4Meta");
	lua_setmetatable(L, -2);
}

static int GctkLuaFnMat4New(lua_State* L) {
	Mat4 mat = MAT4_IDENTITY;
	if (lua_gettop(L) == 4) {
		mat.rows[0] = GctkLuaGetVec4(L, 1);
		mat.rows[1] = GctkLuaGetVec4(L, 2);
		mat.rows[2] = GctkLuaGetVec4(L, 3);
		mat.rows[3] = GctkLuaGetVec4(L, 4);
	} else if (lua_gettop(L) >= 16) {
		for (int i = 0; i < 4; i++) {
			mat.rows[i] = Vec4Create(
				lua_tonumber(L, (i * 4) + 1),
				lua_tonumber(L, (i * 4) + 2),
				lua_tonumber(L, (i * 4) + 3),
				lua_tonumber(L, (i * 4) + 4)
			);
		}
	}

	GctkLuaPushMat4(L, mat);
	return 1;
}
static int GctkLuaFnMat4Identity(lua_State* L) {
	GctkLuaPushMat4(L, MAT4_IDENTITY);
	return 1;
}
static int GctkLuaFnMat4CreateOrtho(lua_State* L) {
	float x = (float)luaL_checknumber(L, 1);
	float y = (float)luaL_checknumber(L, 2);
	float w = (float)luaL_checknumber(L, 3);
	float h = (float)luaL_checknumber(L, 4);
	float n = (float)luaL_checknumber(L, 5);
	float f = (float)luaL_checknumber(L, 6);

	GctkLuaPushMat4(L, Mat4CreateOrtho(x, y, w, h, n, f));
	return 1;
}
static int GctkLuaFnMat4CreatePerspective(lua_State* L) {
	float fov = (float)luaL_checknumber(L, 1);
	float w = (float)luaL_checknumber(L, 2);
	float h = (float)luaL_checknumber(L, 3);
	float n = (float)luaL_checknumber(L, 4);
	float f = (float)luaL_checknumber(L, 5);

	GctkLuaPushMat4(L, Mat4CreateProjection(fov, w, h, n, f));
	return 1;
}
static int GctkLuaFnMat4CreateTransform(lua_State* L) {
	Mat4 mat = MAT4_IDENTITY;

	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);
	luaL_checktype(L, 3, LUA_TTABLE);
	luaL_checktype(L, 4, LUA_TTABLE);

	Vec3 pos = GctkLuaGetVec3(L, 1);
	Vec3 centre = GctkLuaGetVec3(L, 2);
	Vec3 size = GctkLuaGetVec3(L, 3);
	Quat rotation = GctkLuaGetQuat(L, 4);

	Mat4Translate(&mat, pos);
	Mat4Translate(&mat, centre);
	Mat4Rotate(&mat, rotation);
	Mat4Translate(&mat, Vec3Neg(centre));
	Mat4Scale(&mat, size);

	GctkLuaPushMat4(L, mat);
	return 1;
}
static int GctkLuaFnMat4Transpose(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	GctkLuaPushMat4(L, Mat4Transpose(GctkLuaGetMat4(L, 1)));
	return 1;
}
static int GctkLuaFnMat4Column(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_Integer idx = luaL_checkinteger(L, 2);
	if (idx >= 1 && idx <= 4) {
		GctkLuaPushVec4(L, Mat4GetColumn(GctkLuaGetMat4(L, 1), (int)idx));
	} else {
		luaL_error(L, "Index out of range! Expected 1 to 4.");
		return 0;
	}

	return 1;
}
static int GctkLuaFnMat4GetTranslation(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	GctkLuaPushVec3(L, Mat4GetTranslation(GctkLuaGetMat4(L, 1)));
	return 1;
}
static int GctkLuaFnMat4GetRotation(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	GctkLuaPushQuat(L, Mat4GetRotation(GctkLuaGetMat4(L, 1)));
	return 1;
}
static int GctkLuaFnMat4GetRotationAxisAngle(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	GctkLuaPushAxisAngle(L, AxisAngleFromQuat(Mat4GetRotation(GctkLuaGetMat4(L, 1))));
	return 1;
}
static int GctkLuaFnMat4GetScale(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	GctkLuaPushVec3(L, Mat4GetScale(GctkLuaGetMat4(L, 1)));
	return 1;
}
static int GctkLuaFnMat4Translate(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);

	Mat4 mat = GctkLuaGetMat4(L, 1);
	Vec3 trans = GctkLuaGetVec3(L, 2);
	Mat4Translate(&mat, trans);
	GctkLuaPushMat4(L, mat);

	return 1;
}
static int GctkLuaFnMat4Rotate(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);

	Mat4 mat = GctkLuaGetMat4(L, 1);
	Quat quat = GctkLuaGetQuat(L, 2);
	Mat4Rotate(&mat, quat);
	GctkLuaPushMat4(L, mat);

	return 1;
}
static int GctkLuaFnMat4Scale(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);

	Mat4 mat = GctkLuaGetMat4(L, 1);
	Vec3 scale = GctkLuaGetVec3(L, 2);
	Mat4Scale(&mat, scale);
	GctkLuaPushMat4(L, mat);

	return 1;
}
static int GctkLuaFnMat4Mul(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);

	GctkLuaPushMat4(L, Mat4Mul(GctkLuaGetMat4(L, 1), GctkLuaGetMat4(L, 2)));
	return 1;
}
static int GctkLuaFnMat4Eq(lua_State* L) {
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TTABLE);

	lua_pushboolean(L, Mat4Eq(GctkLuaGetMat4(L, 1), GctkLuaGetMat4(L, 2)));
	return 1;
}