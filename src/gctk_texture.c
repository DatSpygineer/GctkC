#include "gctk_texture.h"
#include "gctk_debug.h"

#include <GL/glew.h>
#include <SOIL2/SOIL2.h>

GCTK_CONST_IMPL Texture NULL_TEXTURE = { 0 };

static size_t GctkGetPixelSizeByFormat(TextureFormat format) {
	switch (format) {
		case GCTK_BC4: case GCTK_LUMINANCE: return 1;
		case GCTK_BC5: case GCTK_LUMINANCE_ALPHA: return 2;
		case GCTK_BC1: case GCTK_RGB: return 3;
		default: return 4;
	}
}
static size_t GctkGetPixelCount(TextureInfo info) {
	size_t count = info.width;
	if (info.target > GCTK_TEXTURE_1D && info.target != GCTK_TEXTURE_1D_ARRAY) {
		count *= info.height;
	}
	if (info.target == GCTK_TEXTURE_3D) {
		count *= info.depth;
	}
	if (info.target >= GCTK_TEXTURE_1D_ARRAY) {
		count *= info.frame_count;
	}
	return count;
}

static bool GctkLoadRawTexture(const uint8_t* image_data, TextureInfo info, Texture* texture) {
	glGenTextures(1, &texture->id);
	if (texture->id == 0) {
		GCTK_LOG_ERR(GCTK_ERROR_LOAD_TEXTURE, "Failed to generate GL texture!");
		return false;
	}

	GLuint target;
	switch (info.target) {
		case GCTK_TEXTURE_1D: target = GL_TEXTURE_1D; break;
		case GCTK_TEXTURE_2D: target = GL_TEXTURE_2D; break;
		case GCTK_TEXTURE_3D: target = GL_TEXTURE_3D; break;
		case GCTK_TEXTURE_CUBEMAP: target = GL_TEXTURE_CUBE_MAP; break;
		case GCTK_TEXTURE_1D_ARRAY: target = GL_TEXTURE_1D_ARRAY; break;
		case GCTK_TEXTURE_2D_ARRAY: target = GL_TEXTURE_2D_ARRAY; break;
		default: {
			glDeleteTextures(1, &texture->id);
			GCTK_LOG_ERR(GCTK_ERROR_LOAD_TEXTURE, "Failed to load texture: Invalid texture target: %u", info.target);
			return false;
		}
	}

	glBindTexture(target, texture->id);

	GLuint format;
	GLint int_format;
	switch (info.format) {
		case GCTK_LUMINANCE: {
			format = GL_LUMINANCE;
			int_format = GL_LUMINANCE;
		} break;
		case GCTK_LUMINANCE_ALPHA: {
			format = GL_LUMINANCE_ALPHA;
			int_format = GL_LUMINANCE_ALPHA;
		} break;
		case GCTK_RGB: {
			format = GL_RGB;
			int_format = GL_RGB;
		} break;
		default: case GCTK_RGBA: {
			format = GL_RGBA;
			int_format = GL_RGBA;
		} break;
		case GCTK_BC1: {
			format = GL_RGB;
			int_format = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
		} break;
		case GCTK_BC2: {
			format = GL_RGBA;
			int_format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		} break;
		case GCTK_BC3: {
			format = GL_RGBA;
			int_format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		} break;
		case GCTK_BC4: {
			format = GL_LUMINANCE;
			int_format = GL_COMPRESSED_RED_RGTC1_EXT;
		} break;
		case GCTK_BC5: {
			format = GL_LUMINANCE_ALPHA;
			int_format = GL_COMPRESSED_RED_GREEN_RGTC2_EXT;
		} break;
		case GCTK_BC7: {
			format = GL_RGBA;
			int_format = GL_COMPRESSED_RGBA_BPTC_UNORM;
		} break;
		case GCTK_PVRTC2: {
			format = GL_RGBA;
			int_format = GL_COMPRESSED_RGBA_PVRTC_4BPPV2_IMG;
		} break;
	}

	glTexParameteri(target, GL_TEXTURE_WRAP_R, info.clamp_r ? GL_CLAMP : GL_REPEAT);
	glTexParameteri(target, GL_TEXTURE_WRAP_S, info.clamp_s ? GL_CLAMP : GL_REPEAT);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, info.clamp_t ? GL_CLAMP : GL_REPEAT);

	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, info.filter ? GL_LINEAR : GL_NEAREST);
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER,
					info.mipmaps ?
					(info.filter ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_NEAREST) :
					(info.filter ? GL_LINEAR : GL_NEAREST)
	);

	switch (target) {
		case GL_TEXTURE_1D: {
			glTexImage1D(target, 0, int_format, info.width, 0, format, GL_UNSIGNED_BYTE, image_data);
		} break;
		default: case GL_TEXTURE_2D: {
			glTexImage2D(target, 0, int_format, info.width, info.height, 0, format, GL_UNSIGNED_BYTE, image_data);
		} break;
		case GL_TEXTURE_3D: {
			glTexImage3D(target, 0, int_format, info.width, info.height, info.depth, 0, format, GL_UNSIGNED_BYTE, image_data);
		} break;
		case GL_TEXTURE_CUBE_MAP: {
			int w = info.width;
			int h = info.height;
			int x_step = 0, y_step = 0;
			if (info.width == info.height * 6) {
				w /= 6;
				x_step = 1;
			} else if (info.height == info.width * 6) {
				h /= 6;
				y_step = 1;
			} else {
				glDeleteTextures(1, &texture->id);
				texture->id = 0;
				GCTK_LOG_ERR(GCTK_ERROR_LOAD_TEXTURE, "Failed to load cubemap texture: Expected 1/6 size ratio!");
				return false;
			}

			int x_offset = 0, y_offset = 0;
			for (int i = 0; i < 6; i++) {
				glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
								x_offset * w, y_offset * h,
								w, h, format, GL_UNSIGNED_BYTE, image_data);
				x_offset += x_step;
				y_offset += y_step;
			}
		} break;
		case GL_TEXTURE_1D_ARRAY: {
			glTexSubImage2D(target, 0, 0, 0, info.width,
								info.frame_count, format, GL_UNSIGNED_BYTE, image_data);
		} break;
		case GL_TEXTURE_2D_ARRAY: {
			glTexSubImage3D(target, 0, 0, 0, 0, info.width, info.height,
								info.frame_count, format, GL_UNSIGNED_BYTE, image_data);
		} break;
	}

	if (info.mipmaps) {
		glGenerateMipmap(target);
	}

	glBindTexture(target, 0);
	texture->target = target;

	return true;
}

bool GctkCreateTexture(const uint8_t* data, size_t data_size, TextureSettings settings, Texture* texture) {
	int w, h, c;
	uint8_t* image_data = SOIL_load_image_from_memory(data, data_size, &w, &h, &c, SOIL_LOAD_AUTO);
	TextureInfo info = {
		.width = w,
		.height = h,
		.depth = 0,
		.frame_count = 0,
		.target = settings.is_cubemap ? GCTK_TEXTURE_CUBEMAP : GCTK_TEXTURE_2D,
		.filter = settings.filter,
		.clamp_r = false,
		.clamp_s = settings.clamp_s,
		.clamp_t = settings.clamp_t,
		.mipmaps = settings.mipmaps,
		.format = (c > 0) ? (c - 1) : GCTK_RGBA
	};

	bool result = GctkLoadRawTexture(image_data, info, texture);
	SOIL_free_image_data(image_data);
	return result;
}

bool GctkCreateTextureFromFile(const char* path, TextureSettings settings, Texture* texture) {
	int w, h, c;
	uint8_t* image_data = SOIL_load_image(path, &w, &h, &c, SOIL_LOAD_AUTO);
	if (image_data == NULL) {
		GCTK_LOG_ERR(GCTK_ERROR_LOAD_TEXTURE, "Failed to load texture: Texture data is NULL pointer!");
		return false;
	}

	TextureInfo info = {
			.width = w,
			.height = h,
			.depth = 0,
			.frame_count = 0,
			.target = GCTK_TEXTURE_2D,
			.filter = settings.filter,
			.clamp_r = false,
			.clamp_s = settings.clamp_s,
			.clamp_t = settings.clamp_t,
			.mipmaps = settings.mipmaps,
			.format = (c > 0) ? (c - 1) : GCTK_RGBA
	};
	bool result = GctkLoadRawTexture(image_data, info, texture);
	SOIL_free_image_data(image_data);
	return result;
}

bool GctkLoadTexture(const uint8_t* data, size_t data_size, Texture* texture) {
	TextureInfo info = { 0 };
	if (*((uint32_t*)data) != GTEX_IDENTIFIER) {
		GCTK_LOG_ERR(GCTK_ERROR_LOAD_TEXTURE, "Failed to load texture: Identifier is missing!");
		return false;
	}

	size_t offset = 4;
	info.flags = *(data + offset);
	offset++;
	info.format = *(data + offset);
	offset++;

	info.width = *((uint16_t*)(data + offset));
	offset += 2;
	if (info.target > GCTK_TEXTURE_1D) {
		info.height = *((uint16_t*)(data + offset));
		offset += 2;
	}
	if (info.target == GCTK_TEXTURE_3D) {
		info.depth = *((uint16_t*)(data + offset));
		offset += 2;
	}
	if (info.target >= GCTK_TEXTURE_1D_ARRAY) {
		info.frame_count = *((uint16_t*)(data + offset));
		offset += 2;
	}

	return GctkLoadRawTexture(data + offset, info, texture);
}

bool GctkLoadTextureFromFile(const char* path, Texture* texture) {
	FILE* f = fopen(path, "r");
	if (f == NULL) {
		GCTK_LOG_ERR(GCTK_ERROR_FILE_OPEN, "Could not open texture file \"%s\": %s", path, strerror(errno));
		return false;
	}
	TextureInfo info;
	fread(&info, sizeof(TextureInfo), 1, f);
	if (ferror(f)) {
		GCTK_LOG_ERR(GCTK_ERROR_FILE_READ, "Failed to read header data from texture file \"%s\"!", path);
		fclose(f);
		return false;
	}

	size_t pos = ftell(f);
	fseek(f, 0, SEEK_END);
	size_t end = ftell(f);
	fseek(f, (long)pos, SEEK_SET);

	size_t image_size = end - pos;
	uint8_t* image_data = (uint8_t*)malloc(image_size);
	if (image_data == NULL) {
		GCTK_LOG_ERR(GCTK_ERROR_OUT_OF_MEMORY, "Failed to allocate image data for texture file \"%s\"!", path);
		fclose(f);
		return false;
	}
	fread(image_data, image_size, 1, f);
	fclose(f);

	bool result = GctkLoadRawTexture(image_data, info, texture);
	free(image_data);
	return result;
}

void GctkFreeTexture(Texture* texture) {
	if (texture) {
		glDeleteTextures(1, &texture->id);
		texture->id = 0;
	}
}

void GctkApplyTexture(const Texture* texture) {
	if (texture) {
		if (texture->id > 0) {
			glActiveTexture(GL_TEXTURE0 + (texture->id - 1));
		}
		glBindTexture(texture->target, texture->id);
	}
}