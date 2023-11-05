#include "gctk.h"

static Texture tex_test;
static Shader shd_test;
static Sprite spr_test;

static const char* BASIC_VERTEX =	"#version 460 core\n"
									"layout (location = 0) in vec4 aPos;\n"
									"out vec2 UV;\n"
									"\n"
									"uniform mat4 uModel;\n"
									"uniform mat4 uProjection;\n"
									"\n"
									"void main() {\n"
									"   gl_Position = vec4(aPos.xy, 0.0, 1.0);\n"
									"   UV = aPos.zw;\n"
									"}\n";

static const char* BASIC_FRAGMENT = "#version 460 core\n"
									"in vec2 UV;\n"
									"out vec4 FragColor;\n"
									"\n"
									"uniform sampler2D MainTexture;\n"
									"uniform vec4 ColorTint;\n"
									"\n"
									"void main() {\n"
									"    FragColor = texture(MainTexture, UV) * ColorTint;\n"
									"}\n";

void on_init() {
	if (!GctkCreateTextureFromFile("apple.png", (TextureSettings) {
		.filter = false
	}, &tex_test)) GctkCrash();
	if (!GctkCompileShader(BASIC_VERTEX, BASIC_FRAGMENT, &shd_test)) GctkCrash();
	if (!GctkCreateSprite(&shd_test, &tex_test, &spr_test)) GctkCrash();
}
void on_update(double delta) { }
void on_render(double delta) {
	GctkDrawSprite(&spr_test, VEC3_ZERO, VEC3_ONE, QUAT_IDENTITY, COLOR_WHITE);
}
void on_closed() {
	GctkFreeTexture(&tex_test);
	GctkFreeShader(&shd_test);
	GctkFreeSprite(&spr_test);
}

int main(int argc, char** argv) {
	GctkSetupCallbacks(&on_init, &on_update, &on_render, &on_closed, NULL);
	ErrorCode error;
	if ((error = GctkInit("Test", "Spygineer", argc, argv)) != GCTK_NO_ERROR) return error;
	while(GctkUpdate());
	return GctkQuit();
}