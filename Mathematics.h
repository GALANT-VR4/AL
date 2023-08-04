#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate);
Vector3 TransformNomal(const Vector3& v, const Matrix4x4& m);