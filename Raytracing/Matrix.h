#pragma once

#include "Vec3.h"
#include <array>

struct Matrix
{

	Matrix() = default;
	Matrix(float num);
	Matrix(const Matrix& mat);


	void operator=(const Matrix& mat) { data = mat.data; }
	Matrix& operator +=(const Matrix& mat);
	Matrix& operator -=(const Matrix& mat);
	Matrix& operator *=(const Matrix& mat);


	static const Matrix& Translate(const Matrix& mat, const Vec3& vec);

	std::array<std::array<float, 4>, 4> data
	{ {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}};
};

