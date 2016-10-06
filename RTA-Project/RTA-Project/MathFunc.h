#ifndef _MATHFUNC_H_
#define _MATHFUNC_H_

#include "Defines.h"
#include <DirectXMath.h>
using namespace DirectX;


unsigned int Convert2D_1D(unsigned int x, unsigned int y, unsigned int width);

FLOAT3 Mult_Vec3(FLOAT3, MATRIX3X3);

FLOAT3 Subtract(FLOAT3 a, FLOAT3 b);

FLOAT4 Mult_Vertex3x3(FLOAT4 ver, MATRIX3X3 mat);

FLOAT4 Mult_Vertex4x4(FLOAT4 ver, MATRIX4X4 mat4);

FLOAT4 Negate_Vec3(FLOAT4 A);

FLOAT4 Subtract_F4(FLOAT4 A, FLOAT4 B);

MATRIX4X4 XMConverter(XMMATRIX& A);

XMMATRIX XMConverter(MATRIX4X4 A);

MATRIX3X3 Transpose_3X3(MATRIX4X4 A);

MATRIX4X4 Transpose_4x4(MATRIX4X4 A);

void Mat_Identity(MATRIX4X4& A);

MATRIX3X3 Mult_3x3(MATRIX3X3 A, MATRIX3X3 B);

MATRIX4X4 Mult_4x4(MATRIX4X4 A, MATRIX4X4 B);

void Scale_4x4(MATRIX4X4& A, float x, float y, float z);

void RotateX(MATRIX3X3& A, float radians);

void RotateY(MATRIX3X3& A, float radians);

void RotateZ(MATRIX3X3& A, float radians);

void RotateX(MATRIX4X4& A, float radians);

void RotateY(MATRIX4X4& A, float radians);

void RotateZ(MATRIX4X4& A, float radians);

void RotateX_Local(MATRIX4X4& A, float dt);

void RotateY_Local(MATRIX4X4& A, float dt);

void RotateZ_Local(MATRIX4X4& A, float dt);

void Translate(MATRIX4X4& A, float x, float y, float z);

MATRIX4X4 FastInverse(MATRIX4X4 Mat);

MATRIX4X4 CreateViewMatrix(FLOAT4 EyePos, FLOAT4 FocusPos, FLOAT4 UpDir);

MATRIX4X4 CreateProjectionMatrix(float zfar, float znear, float fov, float ar);

#endif