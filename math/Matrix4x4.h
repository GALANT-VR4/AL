#pragma once
/// <summary>
/// 4x4行列
/// </summary>
struct Matrix4x4 final {
	float m[4][4];
	Matrix4x4 operator*(const Matrix4x4& m2) {
		Matrix4x4 result;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				float buf = .0f;
				for (int k = 0; k < 4; k++) {
					buf += m[i][k] * m2.m[k][j];
				}
				result.m[i][j] = buf;
			}
		}
		return result;
	}
};
