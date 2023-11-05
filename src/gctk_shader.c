#include "gctk_shader.h"
#include "gctk_debug.h"

#include <GL/glew.h>

static GLuint GctkCompileShaderPartial(GLenum shader_type, const char* src) {
	int success;
	GLuint shader = glCreateShader(shader_type);
	if (shader == 0) {
		GCTK_LOG_ERR(GCTK_ERROR_SHADER_COMPILE, "Failed to generate %s shader! (GL error %u)",
				shader_type == GL_VERTEX_SHADER ? "vertex" : (shader_type == GL_FRAGMENT_SHADER ? "fragment" : "compute"),
				glGetError()
		);
		return 0;
	}

	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	char log[256] = { 0 };
	if (success == 0) {
		glGetShaderInfoLog(shader, 256, NULL, log);
		GCTK_LOG_ERR(GCTK_ERROR_SHADER_COMPILE, "Failed to compile %s shader: %s",
			shader_type == GL_VERTEX_SHADER ? "vertex" : (shader_type == GL_FRAGMENT_SHADER ? "fragment" : "compute"),
			log
		);
		return 0;
	} else {
		glGetShaderInfoLog(shader, 256, NULL, log);
		if (strlen(log) > 0) {
			GCTK_LOG_WARN("Successfully compiled %s shader with warnings: %s",
					 shader_type == GL_VERTEX_SHADER ? "vertex" : (shader_type == GL_FRAGMENT_SHADER ? "fragment" : "compute"),
					 log
			);
		} else {
			GCTK_LOG("Successfully compiled %s shader",
				shader_type == GL_VERTEX_SHADER ? "vertex" : (shader_type == GL_FRAGMENT_SHADER ? "fragment" : "compute")
			);
		}
	}
	return shader;
}

bool GctkCompileShader(const char* vertex, const char* fragment, Shader* shader) {
	int success;
	GLuint vert = GctkCompileShaderPartial(GL_VERTEX_SHADER, vertex);
	if (vert == 0) return false;
	GLuint frag = GctkCompileShaderPartial(GL_FRAGMENT_SHADER, fragment);
	if (frag == 0) return false;

	shader->id  = glCreateProgram();
	if (shader->id  == 0) {
		GCTK_LOG_ERR(GCTK_ERROR_SHADER_COMPILE, "Failed to generate shader program! (GL error %u)", glGetError());
		return false;
	}

	glAttachShader(shader->id , vert);
	glAttachShader(shader->id , frag);
	glLinkProgram(shader->id);
	glGetProgramiv(shader->id , GL_LINK_STATUS, &success);

	char log[256] = { 0 };
	if (success == 0) {
		glGetProgramInfoLog(shader->id , 256, NULL, log);
		glDeleteShader(vert);
		glDeleteShader(frag);
		glDeleteProgram(shader->id);
		shader->id = 0;

		GCTK_LOG_ERR(GCTK_ERROR_SHADER_LINK, "Failed to link shader: %s", log);
		return false;
	} else {
		glGetProgramInfoLog(shader->id , 256, NULL, log);
		if (strlen(log) > 0) {
			GCTK_LOG_WARN("Successfully linked shader with warnings: %s", log);
		} else {
			GCTK_LOG("Successfully linked shader");
		}
	}

	glDeleteShader(vert);
	glDeleteShader(frag);

	shader->is_compute = false;

	return true;
}
bool GctkCompileComputeShader(const char* src, Shader* shader) {
	int success;
	GLuint sh = GctkCompileShaderPartial(GL_COMPUTE_SHADER, src);
	if (sh == 0) return false;

	shader->id  = glCreateProgram();
	if (shader->id  == 0) {
		GCTK_LOG_ERR(GCTK_ERROR_SHADER_COMPILE, "Failed to generate shader program! (GL error %u)", glGetError());
		return false;
	}
	glAttachShader(shader->id, sh);
	glLinkProgram(shader->id);
	glGetProgramiv(shader->id , GL_LINK_STATUS, &success);

	char log[256] = { 0 };
	if (success == 0) {
		glGetProgramInfoLog(shader->id , 256, NULL, log);
		glDeleteShader(sh);
		glDeleteProgram(shader->id);
		shader->id = 0;

		GCTK_LOG_ERR(GCTK_ERROR_SHADER_LINK, "Failed to link shader: %s", log);
		return false;
	} else {
		glGetProgramInfoLog(shader->id , 256, NULL, log);
		if (strlen(log) > 0) {
			GCTK_LOG_WARN("Successfully linked shader with warnings: %s", log);
		} else {
			GCTK_LOG("Successfully linked shader");
		}
	}
	glDeleteShader(sh);

	shader->is_compute = true;

	return true;
}

void GctkFreeShader(Shader* shader) {
	if (shader != NULL) {
		glDeleteProgram(shader->id);
		shader->id = 0;
	}
}

void GctkUseShader(const Shader* shader) {
	if (shader != NULL) {
		glUseProgram(shader->id);
	}
}

bool GctkShaderSetUniformInt(const Shader* shader, const char* name, int32_t value) {
	if (shader == NULL) { GCTK_LOG_ERR(GCTK_ERROR_NULLPTR, "Attempt to set uniform of NULL pointer!"); return false; }
	GLint location = glGetUniformLocation(shader->id, name);
	if (location < 0) {
		GCTK_LOG_WARN("Could not locate shader uniform \"%s\"!", name);
		return false;
	}

	glUniform1i(location, value);

	return true;
}
bool GctkShaderSetUniformInt2(const Shader* shader, const char* name, int32_t x, int32_t y) {
	if (shader == NULL) { GCTK_LOG_ERR(GCTK_ERROR_NULLPTR, "Attempt to set uniform of NULL pointer!"); return false; }
	GLint location = glGetUniformLocation(shader->id, name);
	if (location < 0) {
		GCTK_LOG_WARN("Could not locate shader uniform \"%s\"!", name);
		return false;
	}

	glUniform2i(location, x, y);

	return true;
}
bool GctkShaderSetUniformInt3(const Shader* shader, const char* name, int32_t x, int32_t y, int32_t z) {
	if (shader == NULL) { GCTK_LOG_ERR(GCTK_ERROR_NULLPTR, "Attempt to set uniform of NULL pointer!"); return false; }
	GLint location = glGetUniformLocation(shader->id, name);
	if (location < 0) {
		GCTK_LOG_WARN("Could not locate shader uniform \"%s\"!", name);
		return false;
	}

	glUniform3i(location, x, y, z);

	return true;
}
bool GctkShaderSetUniformInt4(const Shader* shader, const char* name, int32_t x, int32_t y, int32_t z, int32_t w) {
	if (shader == NULL) { GCTK_LOG_ERR(GCTK_ERROR_NULLPTR, "Attempt to set uniform of NULL pointer!"); return false; }
	GLint location = glGetUniformLocation(shader->id, name);
	if (location < 0) {
		GCTK_LOG_WARN("Could not locate shader uniform \"%s\"!", name);
		return false;
	}

	glUniform4i(location, x, y, z, w);

	return true;
}
bool GctkShaderSetUniformUInt(const Shader* shader, const char* name, uint32_t value) {
	if (shader == NULL) { GCTK_LOG_ERR(GCTK_ERROR_NULLPTR, "Attempt to set uniform of NULL pointer!"); return false; }
	GLint location = glGetUniformLocation(shader->id, name);
	if (location < 0) {
		GCTK_LOG_WARN("Could not locate shader uniform \"%s\"!", name);
		return false;
	}

	glUniform1ui(location, value);

	return true;
}
bool GctkShaderSetUniformUInt2(const Shader* shader, const char* name, uint32_t x, uint32_t y) {
	if (shader == NULL) { GCTK_LOG_ERR(GCTK_ERROR_NULLPTR, "Attempt to set uniform of NULL pointer!"); return false; }
	GLint location = glGetUniformLocation(shader->id, name);
	if (location < 0) {
		GCTK_LOG_WARN("Could not locate shader uniform \"%s\"!", name);
		return false;
	}

	glUniform2ui(location, x, y);

	return true;
}
bool GctkShaderSetUniformUInt3(const Shader* shader, const char* name, uint32_t x, uint32_t y, uint32_t z) {
	if (shader == NULL) { GCTK_LOG_ERR(GCTK_ERROR_NULLPTR, "Attempt to set uniform of NULL pointer!"); return false; }
	GLint location = glGetUniformLocation(shader->id, name);
	if (location < 0) {
		GCTK_LOG_WARN("Could not locate shader uniform \"%s\"!", name);
		return false;
	}

	glUniform3ui(location, x, y, z);

	return true;
}
bool GctkShaderSetUniformUInt4(const Shader* shader, const char* name, uint32_t x, uint32_t y, uint32_t z, uint32_t w) {
	if (shader == NULL) { GCTK_LOG_ERR(GCTK_ERROR_NULLPTR, "Attempt to set uniform of NULL pointer!"); return false; }
	GLint location = glGetUniformLocation(shader->id, name);
	if (location < 0) {
		GCTK_LOG_WARN("Could not locate shader uniform \"%s\"!", name);
		return false;
	}

	glUniform4ui(location, x, y, z, w);

	return true;
}
bool GctkShaderSetUniformFloat(const Shader* shader, const char* name, float value) {
	if (shader == NULL) { GCTK_LOG_ERR(GCTK_ERROR_NULLPTR, "Attempt to set uniform of NULL pointer!"); return false; }
	GLint location = glGetUniformLocation(shader->id, name);
	if (location < 0) {
		GCTK_LOG_WARN("Could not locate shader uniform \"%s\"!", name);
		return false;
	}

	glUniform1f(location, value);

	return true;
}
bool GctkShaderSetUniformFloat2(const Shader* shader, const char* name, float x, float y) {
	if (shader == NULL) { GCTK_LOG_ERR(GCTK_ERROR_NULLPTR, "Attempt to set uniform of NULL pointer!"); return false; }
	GLint location = glGetUniformLocation(shader->id, name);
	if (location < 0) {
		GCTK_LOG_WARN("Could not locate shader uniform \"%s\"!", name);
		return false;
	}

	glUniform2f(location, x, y);

	return true;
}
bool GctkShaderSetUniformFloat3(const Shader* shader, const char* name, float x, float y, float z) {
	if (shader == NULL) { GCTK_LOG_ERR(GCTK_ERROR_NULLPTR, "Attempt to set uniform of NULL pointer!"); return false; }
	GLint location = glGetUniformLocation(shader->id, name);
	if (location < 0) {
		GCTK_LOG_WARN("Could not locate shader uniform \"%s\"!", name);
		return false;
	}

	glUniform3f(location, x, y, z);

	return true;
}
bool GctkShaderSetUniformFloat4(const Shader* shader, const char* name, float x, float y, float z, float w) {
	if (shader == NULL) { GCTK_LOG_ERR(GCTK_ERROR_NULLPTR, "Attempt to set uniform of NULL pointer!"); return false; }
	GLint location = glGetUniformLocation(shader->id, name);
	if (location < 0) {
		GCTK_LOG_WARN("Could not locate shader uniform \"%s\"!", name);
		return false;
	}

	glUniform4f(location, x, y, z, w);

	return true;
}
bool GctkShaderSetUniformVec2(const Shader* shader, const char* name, Vec2 value) {
	if (shader == NULL) { GCTK_LOG_ERR(GCTK_ERROR_NULLPTR, "Attempt to set uniform of NULL pointer!"); return false; }
	GLint location = glGetUniformLocation(shader->id, name);
	if (location < 0) {
		GCTK_LOG_WARN("Could not locate shader uniform \"%s\"!", name);
		return false;
	}

	glUniform2f(location, value.x, value.y);

	return true;
}
bool GctkShaderSetUniformVec3(const Shader* shader, const char* name, Vec3 value) {
	if (shader == NULL) { GCTK_LOG_ERR(GCTK_ERROR_NULLPTR, "Attempt to set uniform of NULL pointer!"); return false; }
	GLint location = glGetUniformLocation(shader->id, name);
	if (location < 0) {
		GCTK_LOG_WARN("Could not locate shader uniform \"%s\"!", name);
		return false;
	}

	glUniform3f(location, value.x, value.y, value.z);

	return true;
}
bool GctkShaderSetUniformVec4(const Shader* shader, const char* name, Vec4 value) {
	if (shader == NULL) { GCTK_LOG_ERR(GCTK_ERROR_NULLPTR, "Attempt to set uniform of NULL pointer!"); return false; }
	GLint location = glGetUniformLocation(shader->id, name);
	if (location < 0) {
		GCTK_LOG_WARN("Could not locate shader uniform \"%s\"!", name);
		return false;
	}

	glUniform4f(location, value.x, value.y, value.z, value.w);

	return true;
}
bool GctkShaderSetUniformColor(const Shader* shader, const char* name, Color value) {
	if (shader == NULL) { GCTK_LOG_ERR(GCTK_ERROR_NULLPTR, "Attempt to set uniform of NULL pointer!"); return false; }
	GLint location = glGetUniformLocation(shader->id, name);
	if (location < 0) {
		GCTK_LOG_WARN("Could not locate shader uniform \"%s\"!", name);
		return false;
	}

	glUniform4f(location, value.r, value.g, value.b, value.a);

	return true;
}

bool GctkShaderSetUniformMat4(const Shader* shader, const char* name, Mat4 value) {
	if (shader == NULL) { GCTK_LOG_ERR(GCTK_ERROR_NULLPTR, "Attempt to set uniform of NULL pointer!"); return false; }
	GLint location = glGetUniformLocation(shader->id, name);
	if (location < 0) {
		GCTK_LOG_WARN("Could not locate shader uniform \"%s\"!", name);
		return false;
	}

	glUniformMatrix4fv(location, 16, GL_FALSE, value.items);

	return true;
}
bool GctkShaderSetUniformTexture(const Shader* shader, const char* name, const Texture* value) {
	if (shader == NULL) { GCTK_LOG_ERR(GCTK_ERROR_NULLPTR, "Attempt to set uniform of NULL pointer!"); return false; }
	GLint location = glGetUniformLocation(shader->id, name);
	if (location < 0) {
		GCTK_LOG_WARN("Could not locate shader uniform \"%s\"!", name);
		return false;
	}

	glUniform1ui(location, value ? value->id : 0);

	return true;
}

bool GctkShaderGetUniformInt(const Shader* shader, const char* name, int32_t* values) {
	if (shader == NULL) { GCTK_LOG_ERR(GCTK_ERROR_NULLPTR, "Attempt to get uniform of NULL pointer!"); return false; }
	if (values == NULL) { GCTK_LOG_ERR(GCTK_ERROR_NULLPTR, "Failed to get uniform: Output pointer is NULL!"); return false; }
	GLint location = glGetUniformLocation(shader->id, name);
	if (location < 0) {
		GCTK_LOG_WARN("Could not locate shader uniform \"%s\"!", name);
		return false;
	}

	glGetUniformiv(shader->id, location, values);

	return true;
}
bool GctkShaderGetUniformUInt(const Shader* shader, const char* name, uint32_t* values) {
	if (shader == NULL) { GCTK_LOG_ERR(GCTK_ERROR_NULLPTR, "Attempt to get uniform of NULL pointer!"); return false; }
	if (values == NULL) { GCTK_LOG_ERR(GCTK_ERROR_NULLPTR, "Failed to get uniform: Output pointer is NULL!"); return false; }
	GLint location = glGetUniformLocation(shader->id, name);
	if (location < 0) {
		GCTK_LOG_WARN("Could not locate shader uniform \"%s\"!", name);
		return false;
	}

	glGetUniformuiv(shader->id, location, values);

	return true;
}
bool GctkShaderGetUniformFloat(const Shader* shader, const char* name, float* values) {
	if (shader == NULL) { GCTK_LOG_ERR(GCTK_ERROR_NULLPTR, "Attempt to get uniform of NULL pointer!"); return false; }
	if (values == NULL) { GCTK_LOG_ERR(GCTK_ERROR_NULLPTR, "Failed to get uniform: Output pointer is NULL!"); return false; }
	GLint location = glGetUniformLocation(shader->id, name);
	if (location < 0) {
		GCTK_LOG_WARN("Could not locate shader uniform \"%s\"!", name);
		return false;
	}

	glGetUniformfv(shader->id, location, values);

	return true;
}
bool GctkShaderGetUniformVec2(const Shader* shader, const char* name, Vec2* value) {
	if (shader == NULL) { GCTK_LOG_ERR(GCTK_ERROR_NULLPTR, "Attempt to get uniform of NULL pointer!"); return false; }
	if (value == NULL) { GCTK_LOG_ERR(GCTK_ERROR_NULLPTR, "Failed to get uniform: Output pointer is NULL!"); return false; }
	GLint location = glGetUniformLocation(shader->id, name);
	if (location < 0) {
		GCTK_LOG_WARN("Could not locate shader uniform \"%s\"!", name);
		return false;
	}

	glGetUniformfv(shader->id, location, value->items);

	return true;
}
bool GctkShaderGetUniformVec3(const Shader* shader, const char* name, Vec3* value) {
	if (shader == NULL) { GCTK_LOG_ERR(GCTK_ERROR_NULLPTR, "Attempt to get uniform of NULL pointer!"); return false; }
	if (value == NULL) { GCTK_LOG_ERR(GCTK_ERROR_NULLPTR, "Failed to get uniform: Output pointer is NULL!"); return false; }
	GLint location = glGetUniformLocation(shader->id, name);
	if (location < 0) {
		GCTK_LOG_WARN("Could not locate shader uniform \"%s\"!", name);
		return false;
	}

	glGetUniformfv(shader->id, location, value->items);

	return true;
}
bool GctkShaderGetUniformVec4(const Shader* shader, const char* name, Vec4* value) {
	if (shader == NULL) { GCTK_LOG_ERR(GCTK_ERROR_NULLPTR, "Attempt to get uniform of NULL pointer!"); return false; }
	if (value == NULL) { GCTK_LOG_ERR(GCTK_ERROR_NULLPTR, "Failed to get uniform: Output pointer is NULL!"); return false; }
	GLint location = glGetUniformLocation(shader->id, name);
	if (location < 0) {
		GCTK_LOG_WARN("Could not locate shader uniform \"%s\"!", name);
		return false;
	}

	glGetUniformfv(shader->id, location, value->items);

	return true;
}
bool GctkShaderGetUniformColor(const Shader* shader, const char* name, Color* value) {
	if (shader == NULL) { GCTK_LOG_ERR(GCTK_ERROR_NULLPTR, "Attempt to get uniform of NULL pointer!"); return false; }
	if (value == NULL) { GCTK_LOG_ERR(GCTK_ERROR_NULLPTR, "Failed to get uniform: Output pointer is NULL!"); return false; }
	GLint location = glGetUniformLocation(shader->id, name);
	if (location < 0) {
		GCTK_LOG_WARN("Could not locate shader uniform \"%s\"!", name);
		return false;
	}

	glGetUniformfv(shader->id, location, value->items);

	return true;
}
bool GctkShaderGetUniformMat4(const Shader* shader, const char* name, Mat4* value) {
	if (shader == NULL) { GCTK_LOG_ERR(GCTK_ERROR_NULLPTR, "Attempt to get uniform of NULL pointer!"); return false; }
	if (value == NULL) { GCTK_LOG_ERR(GCTK_ERROR_NULLPTR, "Failed to get uniform: Output pointer is NULL!"); return false; }
	GLint location = glGetUniformLocation(shader->id, name);
	if (location < 0) {
		GCTK_LOG_WARN("Could not locate shader uniform \"%s\"!", name);
		return false;
	}

	glGetUniformfv(shader->id, location, value->items);

	return true;
}