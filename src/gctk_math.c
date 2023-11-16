#include "gctk_math.h"

#include <time.h>
#include <lua.h>

Vec2 Vec2FromSingle(float value) {
	return (Vec2){ value, value };
}
Vec2 Vec2Add(Vec2 a, Vec2 b) {
	return (Vec2){ a.x + b.x, a.y + b.y };
}
Vec2 Vec2AddScalar(Vec2 a, float b) {
	return (Vec2){ a.x + b, a.y + b };
}
Vec2 Vec2Sub(Vec2 a, Vec2 b) {
	return (Vec2){ a.x - b.x, a.y - b.y };
}
Vec2 Vec2SubScalar(Vec2 a, float b) {
	return (Vec2){ a.x - b, a.y - b };
}
Vec2 Vec2Mul(Vec2 a, Vec2 b) {
	return (Vec2){ a.x * b.x, a.y * b.y };
}
Vec2 Vec2MulScalar(Vec2 a, float b) {
	return (Vec2){ a.x * b, a.y * b };
}
Vec2 Vec2Div(Vec2 a, Vec2 b) {
	return (Vec2){ a.x / b.x, a.y / b.y };
}
Vec2 Vec2DivScalar(Vec2 a, float b) {
	return (Vec2){ a.x / b, a.y / b };
}
Vec2 Vec2Neg(Vec2 value) {
	return (Vec2){ -value.x, -value.y };
}
bool Vec2Eq(Vec2 a, Vec2 b) {
	return glm_eq(a.x, b.x) && glm_eq(a.y, b.y);
}
float Vec2Dot(Vec2 a, Vec2 b) {
	return a.x * b.x + a.y * b.y;
}
float Vec2Dist(Vec2 a, Vec2 b) {
	return glm_vec2_distance(a.items, b.items);
}
float Vec2Length(Vec2 vec) {
	return sqrtf(vec.x * vec.x + vec.y * vec.y);
}
Vec2 Vec2Norm(Vec2 vec) {
	const float len = Vec2Length(vec);
	return len > 0 ? Vec2DivScalar(vec, len) : (Vec2){0.0f };
}

GCTK_CONST_IMPL Vec2 VEC2_ZERO = { 0, 0 };
GCTK_CONST_IMPL Vec2 VEC2_UNIT_X = { 1, 0 };
GCTK_CONST_IMPL Vec2 VEC2_UNIT_Y = { 0, 1 };
GCTK_CONST_IMPL Vec2 VEC2_ONE  = { 1, 1 };

Vec3 Vec3FromSingle(float value) {
	return (Vec3){ value, value, value };
}
Vec3 Vec3Add(Vec3 a, Vec3 b) {
	return (Vec3){ a.x + b.x, a.y + b.y, a.z + b.z };
}
Vec3 Vec3AddScalar(Vec3 a, float b) {
	return (Vec3){ a.x + b, a.y + b, a.z + b };
}
Vec3 Vec3Sub(Vec3 a, Vec3 b) {
	return (Vec3){ a.x - b.x, a.y - b.y, a.z - b.z };
}
Vec3 Vec3SubScalar(Vec3 a, float b) {
	return (Vec3){ a.x - b, a.y - b, a.z - b };
}
Vec3 Vec3Mul(Vec3 a, Vec3 b) {
	return (Vec3){ a.x * b.x, a.y * b.y, a.z * b.z };
}
Vec3 Vec3MulScalar(Vec3 a, float b) {
	return (Vec3){ a.x * b, a.y * b, a.z * b };
}
Vec3 Vec3Div(Vec3 a, Vec3 b) {
	return (Vec3){ a.x / b.x, a.y / b.y, a.z / b.z };
}
Vec3 Vec3DivScalar(Vec3 a, float b) {
	return (Vec3){ a.x / b, a.y / b, a.z / b };
}
Vec3 Vec3Neg(Vec3 value) {
	return (Vec3){ -value.x, -value.y, -value.z };
}
bool Vec3Eq(Vec3 a, Vec3 b) {
	return glm_eq(a.x, b.x) && glm_eq(a.y, b.y) && glm_eq(a.z, b.z);
}
float Vec3Dot(Vec3 a, Vec3 b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}
Vec3 Vec3Cross(Vec3 a, Vec3 b) {
	Vec3 cross = { 0 };
	glm_vec3_cross(a.items, b.items, cross.items);
	return cross;
}
float Vec3Dist(Vec3 a, Vec3 b) {
	return glm_vec3_distance(a.items, b.items);
}
float Vec3Length(Vec3 vec) {
	return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}
Vec3 Vec3Norm(Vec3 vec) {
	const float len = Vec3Length(vec);
	return len > 0 ? Vec3DivScalar(vec, len) : (Vec3){0.0f };
}
Vec2 Vec3XY(Vec3 vec) {
	return Vec2Create(vec.x, vec.y);
}

GCTK_CONST_IMPL Vec3 VEC3_ZERO = { 0, 0, 0 };
GCTK_CONST_IMPL Vec3 VEC3_UNIT_X = { 1, 0, 0 };
GCTK_CONST_IMPL Vec3 VEC3_UNIT_Y = { 0, 1, 0 };
GCTK_CONST_IMPL Vec3 VEC3_UNIT_Z = { 0, 0, 1 };
GCTK_CONST_IMPL Vec3 VEC3_ONE  = { 1, 1, 1 };

Vec4 Vec4FromSingle(float value) {
	return (Vec4){ value, value, value, value };
}
Vec4 Vec4Add(Vec4 a, Vec4 b) {
	return (Vec4){ a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
}
Vec4 Vec4AddScalar(Vec4 a, float b) {
	return (Vec4){ a.x + b, a.y + b, a.z + b, a.w + b };
}
Vec4 Vec4Sub(Vec4 a, Vec4 b) {
	return (Vec4){ a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w };
}
Vec4 Vec4SubScalar(Vec4 a, float b) {
	return (Vec4){ a.x - b, a.y - b, a.z - b, a.w - b };
}
Vec4 Vec4Mul(Vec4 a, Vec4 b) {
	return (Vec4){ a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w };
}
Vec4 Vec4MulScalar(Vec4 a, float b) {
	return (Vec4){ a.x * b, a.y * b, a.z * b, a.w * b };
}
Vec4 Vec4Div(Vec4 a, Vec4 b) {
	return (Vec4){ a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w };
}
Vec4 Vec4DivScalar(Vec4 a, float b) {
	return (Vec4){ a.x / b, a.y / b, a.z / b, a.w / b };
}
Vec4 Vec4Neg(Vec4 value) {
	return (Vec4){ -value.x, -value.y, -value.z, -value.w };
}
bool Vec4Eq(Vec4 a, Vec4 b) {
	return glm_eq(a.x, b.x) && glm_eq(a.y, b.y) && glm_eq(a.z, b.z) && glm_eq(a.w, b.w);
}
float Vec4Dot(Vec4 a, Vec4 b) {
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}
float Vec4Dist(Vec4 a, Vec4 b) {
	return glm_vec4_distance(a.items, b.items);
}
float Vec4Length(Vec4 vec) {
	return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w);
}
Vec4 Vec4Norm(Vec4 vec) {
	return Vec4DivScalar(vec, Vec4Length(vec));
}
Vec3 Vec4XYZ(Vec4 vec) {
	return Vec3Create(vec.x, vec.y, vec.z);
}

Quat QuatFromAxisAngle(AxisAngle aa) {
	Quat q = { 0 };
	glm_quat(q.items, DegToRad(aa.angle), aa.axis.x, aa.axis.y, aa.axis.z);
	return q;
}
AxisAngle AxisAngleFromQuat(Quat q) {
	AxisAngle aa;
	aa.angle = RadToDeg(glm_quat_angle(q.items));
	glm_quat_axis(q.items, aa.axis.items);
	return aa;
}

GCTK_CONST_IMPL Vec4 VEC4_ZERO   = { 0, 0, 0, 0 };
GCTK_CONST_IMPL Vec4 VEC4_UNIT_X = { 1, 0, 0, 0 };
GCTK_CONST_IMPL Vec4 VEC4_UNIT_Y = { 0, 1, 0, 0 };
GCTK_CONST_IMPL Vec4 VEC4_UNIT_Z = { 0, 0, 1, 0 };
GCTK_CONST_IMPL Vec4 VEC4_UNIT_W = { 0, 0, 0, 1 };
GCTK_CONST_IMPL Vec4 VEC4_ONE    = { 1, 1, 1, 1 };

GCTK_CONST_IMPL Color COLOR_TRANSPARENT =	{ 0 };
GCTK_CONST_IMPL Color COLOR_BLACK = 		{ .r = 0.0f, .g = 0.0f, .b = 0.0f, .a = 1.0f };
GCTK_CONST_IMPL Color COLOR_GRAY = 		{ .r = 0.5f, .g = 0.5f, .b = 0.5f, .a = 1.0f };
GCTK_CONST_IMPL Color COLOR_WHITE = 		{ .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };
GCTK_CONST_IMPL Color COLOR_RED = 		{ .r = 1.0f, .g = 0.0f, .b = 0.0f, .a = 1.0f };
GCTK_CONST_IMPL Color COLOR_GREEN = 		{ .r = 0.0f, .g = 1.0f, .b = 0.0f, .a = 1.0f };
GCTK_CONST_IMPL Color COLOR_YELLOW =		{ .r = 1.0f, .g = 1.0f, .b = 0.0f, .a = 1.0f };
GCTK_CONST_IMPL Color COLOR_BLUE = 		{ .r = 0.0f, .g = 0.0f, .b = 1.0f, .a = 1.0f };
GCTK_CONST_IMPL Color COLOR_MAGENTA = 	{ .r = 1.0f, .g = 0.0f, .b = 1.0f, .a = 1.0f };
GCTK_CONST_IMPL Color COLOR_CYAN = 		{ .r = 0.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f };

Color ColorFromBytes(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	return ColorCreate(
		((float)r) / 255.0f,
		((float)g) / 255.0f,
		((float)b) / 255.0f,
		((float)a) / 255.0f
	);
}
Color ColorFromRgb(uint32_t rgb) {
	return ColorFromRgba((rgb << 8) | 0xFF);
}
Color ColorFromRgba(uint32_t rgba) {
	return ColorFromBytes((rgba >> 24) & 0xFF,
						(rgba >> 16) & 0xFF,
						(rgba >> 8) & 0xFF,
						rgba & 0xFF
	);
}
void ColorToBytes(Color color, uint8_t* r, uint8_t* g, uint8_t* b, uint8_t* a) {
	if (r && g && b && a) {
		*r = (uint8_t)Floor(color.r * 255);
		*g = (uint8_t)Floor(color.g * 255);
		*b = (uint8_t)Floor(color.b * 255);
		*a = (uint8_t)Floor(color.a * 255);
	}
}
uint32_t ColorToRgb(Color color) {
	return ((uint8_t)Floor(color.r * 255) << 16) |
	((uint8_t)Floor(color.g * 255) << 8) |
	(uint8_t)Floor(color.b * 255);
}
uint32_t ColorToRgba(Color color) {
	return ((uint8_t)Floor(color.r * 255) << 24) |
	((uint8_t)Floor(color.g * 255) << 16) |
	((uint8_t)Floor(color.b * 255) << 8) |
	((uint8_t)Floor(color.a * 255));
}
Color ColorFromHex(const char* str) {
	if (str != NULL && str[0] != 0) {
		if (str[0] == '#') {
			str++;
		}
		uint32_t hex = strtoul(str, NULL, 16);
		return strlen(str) < 8 ? ColorFromRgb(hex) : ColorFromRgba(hex);
	}
	return COLOR_BLACK;
}

bool ColorEq(Color a, Color b) {
	return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
}

Mat4 Mat4Mul(Mat4 a, Mat4 b) {
	Mat4 result = { 0 };
	glm_mat4_mul(a.cglm, b.cglm, result.cglm);
	return result;
}
Mat4 Mat4Transpose(Mat4 value) {
	Mat4 result = { 0 };
	memcpy(&result, &value, sizeof(Mat4));
	glm_mat4_transpose(result.cglm);
	return result;
}
bool Mat4Eq(Mat4 a, Mat4 b) {
	return	Vec4Eq(a.rows[0], b.rows[0]) && Vec4Eq(a.rows[1], b.rows[1]) &&
			Vec4Eq(a.rows[2], b.rows[2]) && Vec4Eq(a.rows[3], b.rows[3]);
}

Vec4 Mat4GetColumn(Mat4 mat, int idx) {
	if (idx < 0 || idx > 3) return VEC4_ZERO;
	return Vec4Create(mat.rows[0].items[idx], mat.rows[1].items[idx], mat.rows[2].items[idx], mat.rows[3].items[idx]);
}

Vec3 Mat4GetTranslation(Mat4 mat) {
	return Vec4XYZ(mat.rows[3]);
}
Quat Mat4GetRotation(Mat4 mat) {
	// From OpenTK https://github.com/opentk/opentk/blob/master/src/OpenTK.Mathematics/Matrix/Matrix4.cs#L235
	Vec3 row0 = Vec3Norm(Vec4XYZ(mat.rows[0]));
	Vec3 row1 = Vec3Norm(Vec4XYZ(mat.rows[1]));
	Vec3 row2 = Vec3Norm(Vec4XYZ(mat.rows[2]));
	Quat result = QUAT_IDENTITY;

	float trace = 0.25f * (row0.x + row1.y + row2.z + 1.0f);
	float sq;

	if (trace > 0) {
		sq = sqrtf(trace);

		result.w = sq;
		sq = 1.0f / (4.0f * sq);

		result.x = (row1.z - row2.y) * sq;
		result.y = (row2.x - row0.z) * sq;
		result.z = (row0.y - row1.x) * sq;
	} else if (row0.x > row1.y && row0.x > row2.z) {
		sq = 2.0f * sqrtf(1.0f + row0.x - row1.y - row2.z);

		result.x = 0.25f * sq;
		sq = 1.0f / sq;
		result.w = (row2.y - row1.z) * sq;
		result.y = (row1.x - row0.y) * sq;
		result.z = (row2.x - row0.y) * sq;
	} else if (row1.y > row2.y) {
		sq = 2.0f * sqrtf(1.0f + row1.y - row0.x - row2.z);

		result.y = 0.25f * sq;
		sq = 1.0f / sq;
		result.w = (row2.x - row0.z) * sq;
		result.x = (row1.x - row0.y) * sq;
		result.z = (row2.y - row1.z) * sq;
	} else {
		sq = 2.0f * sqrtf(1.0f + row2.z - row0.x - row1.y);

		result.z = (float)(0.25 * sq);
		sq = 1.0f / sq;
		result.w = (row1.x - row0.y) * sq;
		result.x = (row2.x + row0.z) * sq;
		result.y = (row2.y + row1.z) * sq;
	}

	return QuatNorm(result);
}
Vec3 Mat4GetScale(Mat4 mat) {
	return Vec3Create(
		Vec3Length(Vec4XYZ(mat.rows[0])),
		Vec3Length(Vec4XYZ(mat.rows[1])),
		Vec3Length(Vec4XYZ(mat.rows[2]))
	);
}
Vec4 Mat4GetProjection(Mat4 mat) {
	return Mat4GetColumn(mat, 3);
}

Mat4* Mat4Translate(Mat4* mat, Vec3 translation) {
	if (mat != NULL) {
		glm_translate(mat->cglm, translation.items);
	}
	return mat;
}
Mat4* Mat4Scale(Mat4* mat, Vec3 scale) {
	if (mat != NULL) {
		glm_scale(mat->cglm, scale.items);
	}
	return mat;
}
Mat4* Mat4Rotate(Mat4* mat, Quat rotation) {
	return Mat4RotateAA(mat, AxisAngleFromQuat(rotation));
}
Mat4* Mat4RotateAA(Mat4* mat, AxisAngle rotation) {
	if (mat != NULL) {
		glm_rotate(mat->cglm, DegToRad(rotation.angle), rotation.axis.items);
	}

	return mat;
}

Mat4 Mat4CreateOrtho(float x, float y, float width, float height, float nearZ, float farZ) {
	Mat4 projection = { 0 };
	glm_ortho(x, y, x + width, y + height, nearZ, farZ, projection.cglm);
	return projection;
}
Mat4 Mat4CreateProjection(float fov, float width, float height, float nearZ, float farZ) {
	Mat4 projection = { 0 };
	glm_perspective(fov, width / height, nearZ, farZ, projection.cglm);
	return projection;
}

GCTK_CONST_IMPL Mat4 MAT4_IDENTITY = {
	.rows = {
		{ 1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 }
	}
};

float Lerp(float a, float b, float blend) {
	return glm_lerp(a, b, blend);
}
double LerpD(double a, double b, double blend) {
	return b + blend * (b - a);
}
Vec2 Vec2Lerp(Vec2 a, Vec2 b, float blend) {
	Vec2 result;
	glm_vec2_lerp(a.items, b.items, blend, result.items);
	return result;
}
Vec3 Vec3Lerp(Vec3 a, Vec3 b, float blend) {
	Vec3 result;
	glm_vec3_lerp(a.items, b.items, blend, result.items);
	return result;
}
Vec4 Vec4Lerp(Vec4 a, Vec4 b, float blend) {
	Vec4 result;
	glm_vec4_lerp(a.items, b.items, blend, result.items);
	return result;
}
Color ColorLerp(Color a, Color b, float blend) {
	Color result;
	glm_vec4_lerp(a.items, b.items, blend, result.items);
	return result;
}

float Min(float x, float y) {
	return x < y ? x : y;
}
float Max(float x, float y) {
	return x > y ? x : y;
}
float Clamp(float x, float min, float max) {
	return x > max ? max : (x < min ? min : x);
}
double ClampD(double x, double min, double max) {
	return x > max ? max : (x < min ? min : x);
}

long Floor(float x) {
	return (long)floorf(x);
}
long Ceil(float x) {
	return (long)ceilf(x);
}
long Round(float x) {
	return (long)roundf(x);
}

void SeedRandom(uint32_t seed) {
	srand(seed);
}

int RandomInt(int min, int max) {
	return ((rand() % (max - min)) + min);
}
float RandomFloat(float min, float max) {
	return Lerp(min, max, ((float) rand() / (float) RAND_MAX));
}
Vec2 RandomVec2(Vec2 min, Vec2 max) {
	return (Vec2) {
		RandomFloat(min.x, max.x),
		RandomFloat(min.y, max.y)
	};
}
Vec3 RandomVec3(Vec3 min, Vec3 max) {
	return (Vec3) {
		RandomFloat(min.x, max.x),
		RandomFloat(min.y, max.y),
		RandomFloat(min.z, max.z)
	};
}
Vec4 RandomVec4(Vec4 min, Vec4 max) {
	return (Vec4) {
		RandomFloat(min.x, max.x),
		RandomFloat(min.y, max.y),
		RandomFloat(min.z, max.z),
		RandomFloat(min.w, max.w)
	};
}

Mat4 GctkCreateTransform(Vec3 position, Vec3 size, Vec3 scale, Quat rotation) {
	Mat4 result = MAT4_IDENTITY;
	Mat4Translate(&result, position);
	Mat4Translate(&result, Vec3MulScalar(size, 0.5f));
	Mat4Rotate(&result, rotation);
	Mat4Translate(&result, Vec3MulScalar(size, -0.5f));
	Mat4Scale(&result, Vec3Mul(scale, size));
	return result;
}