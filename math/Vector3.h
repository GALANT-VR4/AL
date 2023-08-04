#pragma once

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {
	float x;
	float y;
	float z;
	Vector3 operator+=(Vector3 v) { return {x += v.x, y += v.y, z += v.z}; }
	Vector3 operator+(const Vector3 v) { return {x + v.x, y + v.y, z + v.z}; }
	Vector3 operator-(const Vector3 v) { return {x - v.x, y - v.y, z - v.z}; }
	Vector3 operator*(const float a) { return {x * a, y * a, z * a}; }
};