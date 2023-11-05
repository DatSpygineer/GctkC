#pragma once

#include "gctk_common.h"
#include "gctk_math.h"
#include "gctk_texture.h"

typedef struct Shader {
	uint32_t id;
	bool is_compute;
} Shader;

GCTK_API bool GctkCompileShader(const char* vertex, const char* fragment, Shader* shader);
GCTK_API bool GctkCompileComputeShader(const char* src, Shader* shader);
GCTK_API void GctkFreeShader(Shader* shader);
GCTK_API void GctkUseShader(const Shader* shader);

GCTK_API bool GctkShaderSetUniformInt(const Shader* shader, const char* name, int32_t value);
GCTK_API bool GctkShaderSetUniformInt2(const Shader* shader, const char* name, int32_t x, int32_t y);
GCTK_API bool GctkShaderSetUniformInt3(const Shader* shader, const char* name, int32_t x, int32_t y, int32_t z);
GCTK_API bool GctkShaderSetUniformInt4(const Shader* shader, const char* name, int32_t x, int32_t y, int32_t z, int32_t w);
GCTK_API bool GctkShaderSetUniformUInt(const Shader* shader, const char* name, uint32_t value);
GCTK_API bool GctkShaderSetUniformUInt2(const Shader* shader, const char* name, uint32_t x, uint32_t y);
GCTK_API bool GctkShaderSetUniformUInt3(const Shader* shader, const char* name, uint32_t x, uint32_t y, uint32_t z);
GCTK_API bool GctkShaderSetUniformUInt4(const Shader* shader, const char* name, uint32_t x, uint32_t y, uint32_t z, uint32_t w);
GCTK_API bool GctkShaderSetUniformFloat(const Shader* shader, const char* name, float value);
GCTK_API bool GctkShaderSetUniformFloat2(const Shader* shader, const char* name, float x, float y);
GCTK_API bool GctkShaderSetUniformFloat3(const Shader* shader, const char* name, float x, float y, float z);
GCTK_API bool GctkShaderSetUniformFloat4(const Shader* shader, const char* name, float x, float y, float z, float w);
GCTK_API bool GctkShaderSetUniformVec2(const Shader* shader, const char* name, Vec2 value);
GCTK_API bool GctkShaderSetUniformVec3(const Shader* shader, const char* name, Vec3 value);
GCTK_API bool GctkShaderSetUniformVec4(const Shader* shader, const char* name, Vec4 value);
#define GctkShaderSetUniformQuat GctkShaderSetUniformVec4
GCTK_API bool GctkShaderSetUniformColor(const Shader* shader, const char* name, Color value);
#define GctkShaderSetUniformColour GctkShaderSetUniformColor
GCTK_API bool GctkShaderSetUniformMat4(const Shader* shader, const char* name, Mat4 value);
GCTK_API bool GctkShaderSetUniformTexture(const Shader* shader, const char* name, const Texture* value);

GCTK_API bool GctkShaderGetUniformInt(const Shader* shader, const char* name, int32_t* values);
GCTK_API bool GctkShaderGetUniformUInt(const Shader* shader, const char* name, uint32_t* values);
GCTK_API bool GctkShaderGetUniformFloat(const Shader* shader, const char* name, float* values);
GCTK_API bool GctkShaderGetUniformVec2(const Shader* shader, const char* name, Vec2* value);
GCTK_API bool GctkShaderGetUniformVec3(const Shader* shader, const char* name, Vec3* value);
GCTK_API bool GctkShaderGetUniformVec4(const Shader* shader, const char* name, Vec4* value);
#define GctkShaderGetUniformQuat GctkShaderGetUniformVec4
GCTK_API bool GctkShaderGetUniformColor(const Shader* shader, const char* name, Color* value);
#define GctkShaderGetUniformColour GctkShaderGetUniformColor
GCTK_API bool GctkShaderGetUniformMat4(const Shader* shader, const char* name, Mat4* value);