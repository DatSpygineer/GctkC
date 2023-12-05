#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "gctk_common.h"
#include <cglm/cglm.h>
#include <math.h>

typedef union Vec2 {
	struct { float x, y; };
	float items[2];
} Vec2;
typedef union Vec3 {
	struct { float x, y, z; };
	float items[3];
} Vec3;
typedef union Vec4 {
	struct { float x, y, z, w; };
	float items[4];
} Vec4;
typedef Vec4 Quat;
typedef union AxisAngle {
	struct {
		Vec3 axis;
		float angle;
	};
	Vec4 axis_angle;
	float items[4];
} AxisAngle;

typedef union Color {
	struct { float r, g, b, a; };
	float items[4];
} Color;
typedef Color Colour;

typedef union Mat4 {
	Vec4 rows[4];
	float items[16];
	mat4 cglm;
} Mat4;

#define Vec2Create(__x, __y) ((Vec2){ (__x), (__y) })
#define Vec2Spead(__vec) (__vec).x, (__vec).y

GCTK_API Vec2 Vec2FromSingle(float value);
GCTK_API Vec2 Vec2Add(Vec2 a, Vec2 b);
GCTK_API Vec2 Vec2AddScalar(Vec2 a, float b);
GCTK_API Vec2 Vec2Sub(Vec2 a, Vec2 b);
GCTK_API Vec2 Vec2SubScalar(Vec2 a, float b);
GCTK_API Vec2 Vec2Mul(Vec2 a, Vec2 b);
GCTK_API Vec2 Vec2MulScalar(Vec2 a, float b);
GCTK_API Vec2 Vec2Div(Vec2 a, Vec2 b);
GCTK_API Vec2 Vec2DivScalar(Vec2 a, float b);
GCTK_API Vec2 Vec2Neg(Vec2 value);
GCTK_API bool Vec2Eq(Vec2 a, Vec2 b);
GCTK_API float Vec2Dot(Vec2 a, Vec2 b);
GCTK_API float Vec2Dist(Vec2 a, Vec2 b);
GCTK_API float Vec2Length(Vec2 vec);
GCTK_API Vec2 Vec2Norm(Vec2 vec);

GCTK_CONST Vec2 VEC2_ZERO;
GCTK_CONST Vec2 VEC2_UNIT_X;
GCTK_CONST Vec2 VEC2_UNIT_Y;
GCTK_CONST Vec2 VEC2_ONE;

#define Vec3Create(__x, __y, __z) ((Vec3){ (__x), (__y), (__z) })
#define Vec3Spead(__vec) (__vec).x, (__vec).y, (__vec).z

GCTK_API Vec3 Vec3FromSingle(float value);
GCTK_API Vec3 Vec3Add(Vec3 a, Vec3 b);
GCTK_API Vec3 Vec3AddScalar(Vec3 a, float b);
GCTK_API Vec3 Vec3Sub(Vec3 a, Vec3 b);
GCTK_API Vec3 Vec3SubScalar(Vec3 a, float b);
GCTK_API Vec3 Vec3Mul(Vec3 a, Vec3 b);
GCTK_API Vec3 Vec3MulScalar(Vec3 a, float b);
GCTK_API Vec3 Vec3Div(Vec3 a, Vec3 b);
GCTK_API Vec3 Vec3DivScalar(Vec3 a, float b);
GCTK_API Vec3 Vec3Neg(Vec3 value);
GCTK_API bool Vec3Eq(Vec3 a, Vec3 b);
GCTK_API float Vec3Dot(Vec3 a, Vec3 b);
GCTK_API Vec3 Vec3Cross(Vec3 a, Vec3 b);
GCTK_API float Vec3Dist(Vec3 a, Vec3 b);
GCTK_API float Vec3Length(Vec3 vec);
GCTK_API Vec3 Vec3Norm(Vec3 vec);
GCTK_API Vec2 Vec3XY(Vec3 vec);

GCTK_CONST Vec3 VEC3_ZERO;
GCTK_CONST Vec3 VEC3_UNIT_X;
GCTK_CONST Vec3 VEC3_UNIT_Y;
GCTK_CONST Vec3 VEC3_UNIT_Z;
GCTK_CONST Vec3 VEC3_ONE;

#define Vec4Create(__x, __y, __z, __w) ((Vec4){ (__x), (__y), (__z), (__w) })
#define Vec4Spead(__vec) (__vec).x, (__vec).y, (__vec).z, (__vec).w

GCTK_API Vec4 Vec4FromSingle(float value);
GCTK_API Vec4 Vec4Add(Vec4 a, Vec4 b);
GCTK_API Vec4 Vec4AddScalar(Vec4 a, float b);
GCTK_API Vec4 Vec4Sub(Vec4 a, Vec4 b);
GCTK_API Vec4 Vec4SubScalar(Vec4 a, float b);
GCTK_API Vec4 Vec4Mul(Vec4 a, Vec4 b);
GCTK_API Vec4 Vec4MulScalar(Vec4 a, float b);
GCTK_API Vec4 Vec4Div(Vec4 a, Vec4 b);
GCTK_API Vec4 Vec4DivScalar(Vec4 a, float b);
GCTK_API Vec4 Vec4Neg(Vec4 value);
GCTK_API bool Vec4Eq(Vec4 a, Vec4 b);
GCTK_API float Vec4Dot(Vec4 a, Vec4 b);
GCTK_API float Vec4Dist(Vec4 a, Vec4 b);
GCTK_API float Vec4Length(Vec4 vec);
GCTK_API Vec4 Vec4Norm(Vec4 vec);
GCTK_API Vec3 Vec4XYZ(Vec4 vec);

#define QuatAdd Vec4Add
#define QuatAddScalar Vec4AddScalar
#define QuatSub Vec4Sub
#define QuatSubScalar Vec4SubScalar
#define QuatMul Vec4Mul
#define QuatMulScalar Vec4MulScalar
#define QuatDiv Vec4Div
#define QuatDivScalar Vec4DivScalar
#define QuatNeg Vec4Neg
#define QuatEq Vec4Eq
#define QuatDot Vec4Dot
#define QuatDist Vec4Dist
#define QuatLength Vec4Length
#define QuatNorm Vec4Norm

GCTK_CONST Vec4 VEC4_ZERO;
GCTK_CONST Vec4 VEC4_UNIT_X;
GCTK_CONST Vec4 VEC4_UNIT_Y;
GCTK_CONST Vec4 VEC4_UNIT_Z;
GCTK_CONST Vec4 VEC4_UNIT_W;
GCTK_CONST Vec4 VEC4_ONE;

#define QUAT_ZERO VEC4_ZERO
#define QUAT_UNIT_X VEC4_UNIT_X
#define QUAT_UNIT_Y VEC4_UNIT_Y
#define QUAT_UNIT_Z VEC4_UNIT_Z
#define QUAT_UNIT_W VEC4_UNIT_W
#define QUAT_ONE VEC4_ONE
#define QUAT_IDENTITY VEC4_UNIT_W

#define QuatCreate(__x, __y, __z, __w) ((Quat){ (__x), (__y), (__z), (__w) })
#define QuatSpead(__vec) (__vec).x, (__vec).y, (__vec).z, (__vec).w

GCTK_API Quat QuatFromAxisAngle(AxisAngle aa);
GCTK_API AxisAngle AxisAngleFromQuat(Quat q);
#define AxisAngleFromVec4(vec) ((AxisAngle){ .axis_angle = vec })

#define ColorCreate(__r, __g, __b, __a) ((Color){ (__r), (__g), (__b), (__a) })
#define ColourCreate(__r, __g, __b, __a) ((Color){ (__r), (__g), (__b), (__a) })

#define ColorCreateRGB(__r, __g, __b) ((Color){ (__r), (__g), (__b), 1.0f })
#define ColourCreateRGB(__r, __g, __b) ((Color){ (__r), (__g), (__b), 1.0f })

#define ColorSpread(__color__) (__color__).r, (__color__).g, (__color__).b, (__color__).a
#define ColorSpreadRGB(__color__) (__color__).r, (__color__).g, (__color__).b
#define ColourSpread ColorSpread
#define ColourSpreadRGB ColorSpreadRGB

GCTK_CONST Color COLOR_TRANSPARENT;
GCTK_CONST Color COLOR_BLACK;
GCTK_CONST Color COLOR_GRAY;
GCTK_CONST Color COLOR_WHITE;
GCTK_CONST Color COLOR_RED;
GCTK_CONST Color COLOR_GREEN;
GCTK_CONST Color COLOR_YELLOW;
GCTK_CONST Color COLOR_BLUE;
GCTK_CONST Color COLOR_MAGENTA;
GCTK_CONST Color COLOR_CYAN;

#define COLOUR_TRANSPARENT	COLOR_TRANSPARENT
#define COLOUR_BLACK		COLOR_BLACK
#define COLOUR_GRAY			COLOR_GRAY
#define COLOUR_WHITE		COLOR_WHITE
#define COLOUR_RED			COLOR_RED
#define COLOUR_GREEN		COLOR_GREEN
#define COLOUR_YELLOW		COLOR_YELLOW
#define COLOUR_BLUE			COLOR_BLUE
#define COLOUR_MAGENTA		COLOR_MAGENTA
#define COLOUR_CYAN			COLOR_CYAN

GCTK_API Color ColorFromBytes(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
GCTK_API Color ColorFromRgb(uint32_t rgb);
#define ColourFromRgb ColorFromRgb
GCTK_API Color ColorFromRgba(uint32_t rgba);
#define ColourFromRgba ColorFromRgba
GCTK_API void ColorToBytes(Color color, uint8_t* r, uint8_t* g, uint8_t* b, uint8_t* a);
#define ColourToBytes ColorToBytes
GCTK_API uint32_t ColorToRgb(Color color);
#define ColourToRgb ColorToRgb
GCTK_API uint32_t ColorToRgba(Color color);
#define ColourToRgba ColorToRgba
GCTK_API Color ColorFromHex(const char* str);
GCTK_API bool ColorEq(Color a, Color b);
#define ColourEq ColorEq

GCTK_API Mat4 Mat4Mul(Mat4 a, Mat4 b);
GCTK_API Mat4 Mat4Transpose(Mat4 value);
GCTK_API bool Mat4Eq(Mat4 a, Mat4 b);

GCTK_API Vec4 Mat4GetColumn(Mat4 mat, int idx);
GCTK_API Vec3 Mat4GetTranslation(Mat4 mat);
GCTK_API Quat Mat4GetRotation(Mat4 mat);
GCTK_API Vec3 Mat4GetScale(Mat4 mat);
GCTK_API Vec4 Mat4GetProjection(Mat4 mat);

GCTK_API Mat4* Mat4Translate(Mat4* mat, Vec3 translation);
GCTK_API Mat4* Mat4Scale(Mat4* mat, Vec3 scale);
GCTK_API Mat4* Mat4Rotate(Mat4* mat, Quat rotation);
GCTK_API Mat4* Mat4RotateAA(Mat4* mat, AxisAngle rotation);

GCTK_API Mat4 Mat4CreateOrtho(float x, float y, float width, float height, float nearZ, float farZ);
GCTK_API Mat4 Mat4CreateProjection(float fov, float width, float height, float nearZ, float farZ);

GCTK_CONST Mat4 MAT4_IDENTITY;

typedef union {
	struct { float width, height; };
	Vec2 vec2;
} Size;

GCTK_API bool SizeEq(Size a, Size b);
GCTK_API int8_t SizeCmp(Size self, Size b);

typedef union {
	struct { float x, y, width, height; };
	struct { Vec2 location; Size size; };
} Rect;

GCTK_API bool RectCollisionWithPoint(Rect rect, Vec2 point);
GCTK_API bool RectCollisionWithRect(Rect rect, Rect other);
GCTK_API bool RectEq(Rect a, Rect b);

GCTK_API float Lerp(float a, float b, float blend);
GCTK_API double LerpD(double a, double b, double blend);
GCTK_API Vec2 Vec2Lerp(Vec2 a, Vec2 b, float blend);
GCTK_API Vec3 Vec3Lerp(Vec3 a, Vec3 b, float blend);
GCTK_API Vec4 Vec4Lerp(Vec4 a, Vec4 b, float blend);
GCTK_API Color ColorLerp(Color a, Color b, float blend);

GCTK_API float Min(float x, float y);
GCTK_API float Max(float x, float y);
GCTK_API float Clamp(float x, float min, float max);
GCTK_API double ClampD(double x, double min, double max);

GCTK_API long Floor(float x);
GCTK_API long Ceil(float x);
GCTK_API long Round(float x);

GCTK_API void SeedRandom(uint32_t seed);

GCTK_API int RandomInt(int min, int max);
GCTK_API float RandomFloat(float min, float max);
GCTK_API Vec2 RandomVec2(Vec2 min, Vec2 max);
GCTK_API Vec3 RandomVec3(Vec3 min, Vec3 max);
GCTK_API Vec4 RandomVec4(Vec4 min, Vec4 max);

#define PI 3.14159265359f

#define DegToRad(__deg__) ((__deg__) * (PI / 180.0f))
#define RadToDeg(__rad__) ((__rad__) * (180.0f / PI))

GCTK_API Mat4 GctkCreateTransform(Vec3 position, Vec3 size, Vec3 scale, Quat rotation);

#ifdef __cplusplus
}
#endif