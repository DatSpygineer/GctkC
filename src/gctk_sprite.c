#include "gctk_sprite.h"
#include "gctk_debug.h"

#include <GL/glew.h>

static const float GCTK_SPRITE_VERTS[] = {
	// Verts				// UVs
	0.0f, 1.0f,	0.0f, -1.0f,
	1.0f, 0.0f,	1.0f, 0.0f,
	0.0f, 0.0f,	0.0f, 0.0f,

	0.0f, 1.0f,	0.0f, -1.0f,
	1.0f, 1.0f,	1.0f, -1.0f,
	1.0f, 0.0f,	1.0f, 0.0f
};

bool GctkCreateSprite(Shader* shader, Texture* texture, Sprite* sprite) {
	if (sprite == NULL) {
		GCTK_LOG_ERR(GCTK_ERROR_NULLPTR, "Failed to create sprite: Output pointer is NULL!");
		return false;
	}
	if (shader == NULL) {
		GCTK_LOG_ERR(GCTK_ERROR_NULLPTR, "Failed to create sprite: Shader pointer is NULL!");
		return false;
	}

	glGenVertexArrays(1, &sprite->vao);
	if (sprite->vao == 0) {
		GCTK_LOG_ERR(GCTK_ERROR_OPENGL_RUNTIME, "Failed to generate vertex array! (GL error %u)", glGetError());
		return false;
	}

	glGenBuffers(1, &sprite->vbo);
	if (sprite->vbo == 0) {
		GCTK_LOG_ERR(GCTK_ERROR_OPENGL_RUNTIME, "Failed to generate vertex buffer! (GL error %u)", glGetError());
		return false;
	}

	sprite->shader = shader;
	sprite->texture = texture;

	glBindBuffer(GL_ARRAY_BUFFER, sprite->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GCTK_SPRITE_VERTS), GCTK_SPRITE_VERTS, GL_STATIC_DRAW);
	glBindVertexArray(sprite->vao);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), NULL);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return true;
}
bool GctkDrawSprite(const Sprite* sprite, Vec3 position, Vec3 scale, Quat rotation, Color tint) {
	if (sprite == NULL) {
		GCTK_LOG_ERR(GCTK_ERROR_NULLPTR, "Could not draw sprite: Sprite is NULL pointer!");
		return false;
	}
	if (sprite->shader == NULL) {
		GCTK_LOG_ERR(GCTK_ERROR_NULLPTR, "Could not draw sprite: Sprite's shader is NULL pointer!");
		return false;
	}

	if (sprite->texture != NULL) {
		GctkApplyTexture(sprite->texture);
		GctkShaderSetUniformTexture(sprite->shader, "MainTexture", sprite->texture);
	}

	GctkShaderSetUniformColor(sprite->shader, "ColorTint", tint);
	GctkShaderSetUniformMat4(sprite->shader, "uModel", GctkCreateTransform(
			position,
			sprite->texture != NULL ? Vec3Create((float)sprite->texture->info.width, (float)sprite->texture->info.height, 1.0f) : VEC3_ONE,
			scale,
			rotation
		)
	);
	GctkShaderSetUniformMat4(sprite->shader, "uProjection", MAT4_IDENTITY);
	GctkUseShader(sprite->shader);

	glBindVertexArray(sprite->vao);
	glBindBuffer(GL_ARRAY_BUFFER, sprite->vbo);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return true;
}
void GctkFreeSprite(Sprite* sprite) {
	if (sprite) {
		if (sprite->vao > 0) {
			glDeleteVertexArrays(1, &sprite->vao);
			sprite->vao = 0;
		}
		if (sprite->vbo > 0) {
			glDeleteBuffers(1, &sprite->vbo);
			sprite->vbo = 0;
		}
	}
}