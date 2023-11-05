#pragma once

#include "gctk_common.h"

#define GTEX_IDENTIFIER 0x58455447U

typedef enum TextureTarget {
	GCTK_TEXTURE_1D,			// 000
	GCTK_TEXTURE_2D,			// 001
	GCTK_TEXTURE_3D,			// 010
	GCTK_TEXTURE_CUBEMAP,		// 011
	GCTK_TEXTURE_1D_ARRAY,		// 100
	GCTK_TEXTURE_2D_ARRAY		// 101
	// Unused - 110
	// Unused - 111
} TextureTarget;

typedef enum TextureFormat {
	GCTK_LUMINANCE,
	GCTK_LUMINANCE_ALPHA,
	GCTK_RGB,
	GCTK_RGBA,
	GCTK_BC1,			// RGB
	GCTK_BC2,			// RGBA
	GCTK_BC3,			// RGBA
	GCTK_BC4,			// Luminance
	GCTK_BC5,			// Luminance + Alpha
	GCTK_BC7,			// RGBA
	GCTK_PVRTC2			// RGBA
} TextureFormat;

typedef struct TextureInfo {
	union {
		struct {
			uint8_t clamp_r: 1;
			uint8_t clamp_s: 1;
			uint8_t clamp_t: 1;
			uint8_t filter: 1;
			uint8_t mipmaps: 1;
			uint8_t target: 3;
		};
		uint8_t flags;
	};
	uint8_t format;
	uint16_t width, height, depth, frame_count;
} TextureInfo;

typedef struct TextureSettings {
	bool clamp_s, clamp_t;
	bool filter, mipmaps;
} TextureSettings;

typedef struct Texture {
	uint32_t id, target;
	TextureInfo info;
} Texture;

/**
 * @brief Create GCTk texture from external texture format.
 * @param data Texture data.
 * @param data_size Texture data size in bytes.
 * @param settings Texture parameters.
 * @param texture Result OpenGL texture.
 * @return Return success status.
 */
GCTK_API bool GctkCreateTexture(const uint8_t* data, size_t data_size, TextureSettings settings, Texture* texture);
/**
 * @brief Create GCTk texture from external texture file.
 * @param path Path to the texture file.
 * @param settings Texture parameters.
 * @param texture Result OpenGL texture.
 * @return Return success status.
 */
GCTK_API bool GctkCreateTextureFromFile(const char* path, TextureSettings settings, Texture* texture);
/**
 * @brief Load GCTk texture from memory.
 * @param data Texture data.
 * @param data_size Texture data size in bytes.
 * @param texture Result OpenGL texture.
 * @return Return success status.
 */
GCTK_API bool GctkLoadTexture(const uint8_t* data, size_t data_size, Texture* texture);
/**
 * @brief Load GCTk texture from file.
 * @param path Path to the texture file.
 * @param texture Result OpenGL texture.
 * @return Return success status.
 */
GCTK_API bool GctkLoadTextureFromFile(const char* path, Texture* texture);
/**
 * @brief Destroy texture object.
 */
GCTK_API void GctkFreeTexture(Texture* texture);
/**
 * @brief Use texture.
 * @param texture Texture to use.
 */
GCTK_API void GctkApplyTexture(const Texture* texture);