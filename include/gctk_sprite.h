#pragma once

#include "gctk_common.h"
#include "gctk_texture.h"
#include "gctk_shader.h"

typedef struct Sprite {
	uint32_t vao, vbo;
	Texture* texture;
	Shader* shader;
} Sprite;

GCTK_API bool GctkCreateSprite(Shader* shader, Texture* texture, Sprite* sprite);
GCTK_API bool GctkDrawSprite(const Sprite* sprite, Vec3 position, Vec3 scale, Quat rotation, Color tint);
GCTK_API void GctkFreeSprite(Sprite* sprite);