#include "gctk.h"

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
	GctkLuaRunString("main.lua", "vec = Vec2.New(1, 1)\nDebug.Log(#vec)\nDebug.Log(Math.Sqrt(2))");
}
void on_update(double delta) {
}
void on_render(double delta) {
}
void on_closed() {
}

int main(int argc, char** argv) {
	ErrorCode error;

	GctkSetupCallbacks(&on_init, &on_update, &on_render, &on_closed, NULL);
	if ((error = GctkInit("Test", "Spygineer", argc, argv)) != GCTK_NO_ERROR) return error;
	while(GctkUpdate());
	return GctkQuit();
}