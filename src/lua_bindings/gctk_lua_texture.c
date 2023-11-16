#include "gctk_common.h"
#include "gctk_lua.h"
#include "gctk_texture.h"
#include "gctk_debug.h"

GCTK_API void GctkLuaRegisterTexture(lua_State* L);
GCTK_API void GctkLuaPushTexture(lua_State* L, const Texture* texture);
GCTK_API Texture* GctkLuaGetTexture(lua_State* L, int idx);

static int GctkLuaFnTextureCreate(lua_State* L);
static int GctkLuaFnTextureLoad(lua_State* L);
static int GctkLuaFnTextureEmpty(lua_State* L);

static int GctkLuaFnTextureEq(lua_State* L);
static int GctkLuaFnTextureDrop(lua_State* L);
static int GctkLuaFnTextureToStr(lua_State* L);

static int GctkLuaFnTextureApply(lua_State* L);
static int GctkLuaFnTextureId(lua_State* L);

void GctkLuaRegisterTexture(lua_State* L) {
	lua_newtable(L);
	lua_newtable(L);
	lua_pushcfunction(L, &GctkLuaFnTextureEq);
	lua_setfield(L, -2, "__eq");
	lua_pushcfunction(L, &GctkLuaFnTextureDrop);
	lua_setfield(L, -2, "__gc");
	lua_pushcfunction(L, &GctkLuaFnTextureToStr);
	lua_setfield(L, -2, "__tostring");
	lua_pushstring(L, "Texture");
	lua_setfield(L, -2, "__name");
	lua_pushstring(L, "Texture");
	lua_setfield(L, -2, "__classname");
	lua_setglobal(L, "TextureMeta");

	lua_pushcfunction(L, &GctkLuaFnTextureCreate);
	lua_setfield(L, -2, "Create");
	lua_pushcfunction(L, &GctkLuaFnTextureLoad);
	lua_setfield(L, -2, "Load");
	lua_pushcfunction(L, &GctkLuaFnTextureEmpty);
	lua_setfield(L, -2, "Empty");
	lua_setglobal(L, "Texture");
}

void GctkLuaPushTexture(lua_State* L, const Texture* texture) {
	void* lua_val = lua_newuserdata(L, sizeof(Texture));
	memcpy(lua_val, &texture, sizeof(Texture));
	lua_getglobal(L, "TextureMeta");
	lua_setmetatable(L, -2);
	lua_pushcfunction(L, &GctkLuaFnTextureApply);
	lua_setfield(L, -2, "apply");
	lua_pushcfunction(L, &GctkLuaFnTextureId);
	lua_setfield(L, -2, "id");
}
Texture* GctkLuaGetTexture(lua_State* L, int idx) {
	return (Texture*)luaL_checkudata(L, idx, "Texture");
}

static int GctkLuaFnTextureCreate(lua_State* L) {
	luaL_checktype(L, 2, LUA_TTABLE);

	Texture texture;
	TextureSettings settings = { 0 };

	lua_getfield(L, 2, "clampS");
	settings.clamp_s = lua_isboolean(L, -1) ? lua_toboolean(L, -1) : false;
	lua_pop(L, 1);
	lua_getfield(L, 2, "clampT");
	settings.clamp_t = lua_isboolean(L, -1) ? lua_toboolean(L, -1) : false;
	lua_pop(L, 1);
	lua_getfield(L, 2, "filter");
	settings.filter = lua_isboolean(L, -1) ? lua_toboolean(L, -1) : false;
	lua_pop(L, 1);
	lua_getfield(L, 2, "mipmaps");
	settings.mipmaps = lua_isboolean(L, -1) ? lua_toboolean(L, -1) : false;
	lua_pop(L, 1);
	lua_getfield(L, 2, "isCubemap");
	settings.is_cubemap = lua_isboolean(L, -1) ? lua_toboolean(L, -1) : false;
	lua_pop(L, 1);

	if (!GctkCreateTextureFromFile(luaL_checkstring(L, 1), settings, &texture)) {
		const char* message;
		GctkGetError(&message);
		luaL_error(L, "Failed to load texture! GCTk error: %s", message);
		return 0;
	}
	GctkLuaPushTexture(L, &texture);
	return 1;
}
static int GctkLuaFnTextureLoad(lua_State* L) {
	Texture texture;
	if (!GctkLoadTextureFromFile(luaL_checkstring(L, 1), &texture)) {
		const char* message;
		GctkGetError(&message);
		luaL_error(L, "Failed to load texture! GCTk error: %s", message);
		return 0;
	}
	GctkLuaPushTexture(L, &texture);
	return 1;
}
static int GctkLuaFnTextureEmpty(lua_State* L) {
	GctkLuaPushTexture(L, &NULL_TEXTURE);
	return 1;
}

static int GctkLuaFnTextureEq(lua_State* L) {
	const Texture* a = (const Texture*)luaL_checkudata(L, 1, "Texture");
	const Texture* b = (const Texture*)luaL_checkudata(L, 1, "Texture");
	lua_pushboolean(L, a && b && a->id == b->id);

	return 1;
}
static int GctkLuaFnTextureDrop(lua_State* L) {
	Texture* texture = (Texture*)luaL_checkudata(L, 1, "Texture");
	GctkFreeTexture(texture);
	return 0;
}
static int GctkLuaFnTextureToStr(lua_State* L) {
	const Texture* texture = (const Texture*)luaL_checkudata(L, 1, "Texture");

	char temp[128] = { 0 };
	snprintf(temp, 128, "Texture:%u", texture->id);
	return 1;
}

static int GctkLuaFnTextureApply(lua_State* L) {
	const Texture* texture = (const Texture*)luaL_checkudata(L, 1, "Texture");
	GctkApplyTexture(texture);
	return 0;
}
static int GctkLuaFnTextureId(lua_State* L) {
	const Texture* texture = (const Texture*)luaL_checkudata(L, 1, "Texture");
	lua_pushinteger(L, texture->id);
	return 1;
}
