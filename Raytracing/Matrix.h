#pragma once

#include "Vec3.h"
#include "Vec4.h"
#include <array>

struct Matrix
{

	Matrix();
	Matrix(float num);
	Matrix(const Matrix& mat);


	void operator=(const Matrix& mat) { data = mat.data; }
	Matrix& operator +=(const Matrix& mat);
	Matrix& operator -=(const Matrix& mat);
	Matrix& operator *=(const Matrix& mat);


	static const Matrix& Translate(const Matrix& mat, const Vec3& vec);

	void Set(const Vec3& vec);

	std::array<std::array<float, 4>, 4> data
	{ {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f} };
};

Matrix operator*(const Matrix& m1, const Matrix& m2);

Vec4 operator*(const Matrix& m1, const Vec4& v1);

Vec3 operator*(const Matrix& m1, const Vec3& v1);