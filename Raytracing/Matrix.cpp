#include "Matrix.h"

Matrix::Matrix(float num)
{
	for (size_t i = 0; i < data.size(); i++)
	{
		data[i][i] = num;
	}
}

Matrix::Matrix(const Matrix& mat)
{
	data = mat.data;
}

Matrix& Matrix::operator+=(const Matrix& mat)
{
	for (size_t i = 0; i < data.size(); i++)
	{
		for (size_t j = 0; j < data[i].size(); j++)
		{
			data[i][j] += mat.data[i][j];
		}
	}
	return *this;
}

Matrix& Matrix::operator-=(const Matrix& mat)
{
	for (size_t i = 0; i < data.size(); i++)
	{
		for (size_t j = 0; j < data[i].size(); j++)
		{
			data[i][j] -= mat.data[i][j];
		}
	}
	return *this;
}

Matrix& Matrix::operator*=(const Matrix& mat)
{
	for (size_t i = 0; i < data.size(); i++)
	{
		for (size_t j = 0; j < data[i].size(); j++)
		{
			data[i][j] *= mat.data[j][i];
		}
	}
	return *this;
}

const Matrix& Matrix::Translate(const Matrix& mat, const Vec3& vec)
{
	Matrix m(1.0f);
	m.data[3][0] = mat.data[3][0] + vec.x;
	m.data[3][1] = mat.data[3][1] + vec.y;
	m.data[3][2] = mat.data[3][2] + vec.z;
	return m;
}
