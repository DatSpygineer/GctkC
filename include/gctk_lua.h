#pragma once

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "gctk_common.h"

GCTK_API bool GctkInitLua();
GCTK_API void GctkCloseLua();

GCTK_API bool GctkLuaIsTypeOf(lua_State* L, int idx, const char* name);

GCTK_API bool GctkLuaRunString(const char* name, const char* src);
GCTK_API bool GctkLuaRunFile(const char* path);
