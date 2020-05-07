#include "Matrix.h"

Matrix::Matrix()
{
	for (size_t i = 0; i < data.size(); ++i)
	{
		data[i][i] = 1.0f;
	}
}

Matrix::Matrix(float num)
{
	for (size_t i = 0; i < data.size(); ++i)
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
	for (size_t i = 0; i < data.size(); ++i)
	{
		for (size_t j = 0; j < data[i].size(); ++j)
		{
			data[i][j] += mat.data[i][j];
		}
	}
	return *this;
}

Matrix& Matrix::operator-=(const Matrix& mat)
{
	for (size_t i = 0; i < data.size(); ++i)
	{
		for (size_t j = 0; j < data[i].size(); ++j)
		{
			data[i][j] -= mat.data[i][j];
		}
	}
	return *this;
}

Matrix& Matrix::operator*=(const Matrix& mat)
{
	Matrix m(1.0f);
	for (size_t i = 0; i < data.size(); ++i)
	{
		switch (i)
		{
		case 0:
			m.data[i][0] = (data[i][0] * mat.data[i][0]) + (data[i][1] * mat.data[i + 1][0]) + (data[i][2] * mat.data[i + 2][0]) + (data[i][3] * mat.data[i + 3][0]);
			m.data[i][1] = (data[i][0] * mat.data[i][1]) + (data[i][1] * mat.data[i + 1][1]) + (data[i][2] * mat.data[i + 2][1]) + (data[i][3] * mat.data[i + 3][1]);
			m.data[i][2] = (data[i][0] * mat.data[i][2]) + (data[i][1] * mat.data[i + 1][2]) + (data[i][2] * mat.data[i + 2][2]) + (data[i][3] * mat.data[i + 3][2]);
			m.data[i][3] = (data[i][0] * mat.data[i][3]) + (data[i][1] * mat.data[i + 1][3]) + (data[i][2] * mat.data[i + 2][3]) + (data[i][3] * mat.data[i + 3][3]);
			break;
		case 1:
			m.data[i][0] = (data[i][0] * mat.data[i - 1][0]) + (data[i][1] * mat.data[i][0]) + (data[i][2] * mat.data[i + 1][0]) + (data[i][3] * mat.data[i + 2][0]);
			m.data[i][1] = (data[i][0] * mat.data[i - 1][1]) + (data[i][1] * mat.data[i][1]) + (data[i][2] * mat.data[i + 1][1]) + (data[i][3] * mat.data[i + 2][1]);
			m.data[i][2] = (data[i][0] * mat.data[i - 1][2]) + (data[i][1] * mat.data[i][2]) + (data[i][2] * mat.data[i + 1][2]) + (data[i][3] * mat.data[i + 2][2]);
			m.data[i][3] = (data[i][0] * mat.data[i - 1][3]) + (data[i][1] * mat.data[i][3]) + (data[i][2] * mat.data[i + 1][3]) + (data[i][3] * mat.data[i + 2][3]);
			break;
		case 2:
			m.data[i][0] = (data[i][0] * mat.data[i - 2][0]) + (data[i - 1][1] * mat.data[i - 1][0]) + (data[i][2] * mat.data[i][0]) + (data[i][3] * mat.data[i + 1][0]);
			m.data[i][1] = (data[i][0] * mat.data[i - 2][1]) + (data[i - 1][1] * mat.data[i - 1][1]) + (data[i][2] * mat.data[i][1]) + (data[i][3] * mat.data[i + 1][1]);
			m.data[i][2] = (data[i][0] * mat.data[i - 2][2]) + (data[i - 1][1] * mat.data[i - 1][2]) + (data[i][2] * mat.data[i][2]) + (data[i][3] * mat.data[i + 1][2]);
			m.data[i][3] = (data[i][0] * mat.data[i - 2][3]) + (data[i - 1][1] * mat.data[i - 1][3]) + (data[i][2] * mat.data[i][3]) + (data[i][3] * mat.data[i + 1][3]);
			break;
		case 3:
			m.data[i][0] = (data[i][0] * mat.data[i - 3][0]) + (data[i][1] * mat.data[i - 2][0]) + (data[i][2] * mat.data[i - 1][0]) + (data[i][3] * mat.data[i][0]);
			m.data[i][1] = (data[i][0] * mat.data[i - 3][1]) + (data[i][1] * mat.data[i - 2][1]) + (data[i][2] * mat.data[i - 1][1]) + (data[i][3] * mat.data[i][1]);
			m.data[i][2] = (data[i][0] * mat.data[i - 3][2]) + (data[i][1] * mat.data[i - 2][2]) + (data[i][2] * mat.data[i - 1][2]) + (data[i][3] * mat.data[i][2]);
			m.data[i][3] = (data[i][0] * mat.data[i - 3][3]) + (data[i][1] * mat.data[i - 2][3]) + (data[i][2] * mat.data[i - 1][3]) + (data[i][3] * mat.data[i][3]);
			break;
		}
	}
	data = m.data;
	return *this;
}

const Matrix& Matrix::Translate(const Matrix& mat, const Vec3& vec)
{
	Matrix m(1.0f);
	m.data[3][0] = mat.data[3][0] + vec.axis[0];
	m.data[3][1] = mat.data[3][1] + vec.axis[1];
	m.data[3][2] = mat.data[3][2] + vec.axis[2];

	return m;
}

void Matrix::Set(const Vec3& vec)
{
	data[0][0] = vec.axis[0];
	data[1][1] = vec.axis[1];
	data[2][2] = vec.axis[2];
}

Matrix operator*(const Matrix& m1, const Matrix& m2)
{
	Matrix m(1.0f);
	for (size_t i = 0; i < m1.data.size(); ++i)
	{
		switch (i)
		{
		case 0:
			m.data[i][0] = (m1.data[i][0] * m2.data[i][0]) + (m1.data[i][1] * m2.data[i + 1][0]) + (m1.data[i][2] * m2.data[i + 2][0]) + (m1.data[i][3] * m2.data[i + 3][0]);
			m.data[i][1] = (m1.data[i][0] * m2.data[i][1]) + (m1.data[i][1] * m2.data[i + 1][1]) + (m1.data[i][2] * m2.data[i + 2][1]) + (m1.data[i][3] * m2.data[i + 3][1]);
			m.data[i][2] = (m1.data[i][0] * m2.data[i][2]) + (m1.data[i][1] * m2.data[i + 1][2]) + (m1.data[i][2] * m2.data[i + 2][2]) + (m1.data[i][3] * m2.data[i + 3][2]);
			m.data[i][3] = (m1.data[i][0] * m2.data[i][3]) + (m1.data[i][1] * m2.data[i + 1][3]) + (m1.data[i][2] * m2.data[i + 2][3]) + (m1.data[i][3] * m2.data[i + 3][3]);
			break;																					 										 
		case 1:																						 										 
			m.data[i][0] = (m1.data[i][0] * m2.data[i - 1][0]) + (m1.data[i][1] * m2.data[i][0]) + (m1.data[i][2] * m2.data[i + 1][0]) + (m1.data[i][3] * m2.data[i + 2][0]);
			m.data[i][1] = (m1.data[i][0] * m2.data[i - 1][1]) + (m1.data[i][1] * m2.data[i][1]) + (m1.data[i][2] * m2.data[i + 1][1]) + (m1.data[i][3] * m2.data[i + 2][1]);
			m.data[i][2] = (m1.data[i][0] * m2.data[i - 1][2]) + (m1.data[i][1] * m2.data[i][2]) + (m1.data[i][2] * m2.data[i + 1][2]) + (m1.data[i][3] * m2.data[i + 2][2]);
			m.data[i][3] = (m1.data[i][0] * m2.data[i - 1][3]) + (m1.data[i][1] * m2.data[i][3]) + (m1.data[i][2] * m2.data[i + 1][3]) + (m1.data[i][3] * m2.data[i + 2][3]);
			break;
		case 2:
			m.data[i][0] = (m1.data[i][0] * m2.data[i - 2][0]) + (m1.data[i - 1][1] * m2.data[i - 1][0]) + (m1.data[i][2] * m2.data[i][0]) + (m1.data[i][3] * m2.data[i + 1][0]);
			m.data[i][1] = (m1.data[i][0] * m2.data[i - 2][1]) + (m1.data[i - 1][1] * m2.data[i - 1][1]) + (m1.data[i][2] * m2.data[i][1]) + (m1.data[i][3] * m2.data[i + 1][1]);
			m.data[i][2] = (m1.data[i][0] * m2.data[i - 2][2]) + (m1.data[i - 1][1] * m2.data[i - 1][2]) + (m1.data[i][2] * m2.data[i][2]) + (m1.data[i][3] * m2.data[i + 1][2]);
			m.data[i][3] = (m1.data[i][0] * m2.data[i - 2][3]) + (m1.data[i - 1][1] * m2.data[i - 1][3]) + (m1.data[i][2] * m2.data[i][3]) + (m1.data[i][3] * m2.data[i + 1][3]);
			break;																																
		case 3:																																	
			m.data[i][0] = (m1.data[i][0] * m2.data[i - 3][0]) + (m1.data[i][1] * m2.data[i - 2][0]) + (m1.data[i][2] * m2.data[i - 1][0]) + (m1.data[i][3] * m2.data[i][0]);
			m.data[i][1] = (m1.data[i][0] * m2.data[i - 3][1]) + (m1.data[i][1] * m2.data[i - 2][1]) + (m1.data[i][2] * m2.data[i - 1][1]) + (m1.data[i][3] * m2.data[i][1]);
			m.data[i][2] = (m1.data[i][0] * m2.data[i - 3][2]) + (m1.data[i][1] * m2.data[i - 2][2]) + (m1.data[i][2] * m2.data[i - 1][2]) + (m1.data[i][3] * m2.data[i][2]);
			m.data[i][3] = (m1.data[i][0] * m2.data[i - 3][3]) + (m1.data[i][1] * m2.data[i - 2][3]) + (m1.data[i][2] * m2.data[i - 1][3]) + (m1.data[i][3] * m2.data[i][3]);
			break;
		}
	}
	return m;
}

Vec4 operator*(const Matrix& m, const Vec4& v)
{
	Vec4 vec;
	vec.axis[0] = (m.data[0][0] * v.axis[0]) + (m.data[0][1] * v.axis[0]) + (m.data[0][1] * v.axis[0]) + (m.data[0][3] * v.axis[0]);
	vec.axis[1] = (m.data[1][0] * v.axis[1]) + (m.data[1][1] * v.axis[1]) + (m.data[1][1] * v.axis[1]) + (m.data[1][3] * v.axis[1]);
	vec.axis[2] = (m.data[2][0] * v.axis[2]) + (m.data[2][1] * v.axis[2]) + (m.data[2][1] * v.axis[2]) + (m.data[2][3] * v.axis[2]);
	vec.axis[3] = (m.data[3][0] * v.axis[3]) + (m.data[3][1] * v.axis[3]) + (m.data[3][1] * v.axis[3]) + (m.data[3][3] * v.axis[3]);
	return vec;
}

Vec3 operator*(const Matrix& m, const Vec3& v)
{
	Vec3 vec;
	vec.axis[0] = (m.data[0][0] * v.axis[0]) + (m.data[0][1] * v.axis[0]) + (m.data[0][1] * v.axis[0]) + (m.data[0][3] * v.axis[0]);
	vec.axis[1] = (m.data[1][0] * v.axis[1]) + (m.data[1][1] * v.axis[1]) + (m.data[1][1] * v.axis[1]) + (m.data[1][3] * v.axis[1]);
	vec.axis[2] = (m.data[2][0] * v.axis[2]) + (m.data[2][1] * v.axis[2]) + (m.data[2][1] * v.axis[2]) + (m.data[2][3] * v.axis[2]);
	return vec;
}
