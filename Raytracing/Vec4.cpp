#include "Vec4.h"

Vec4::Vec4(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

Vec4::Vec4(const Vec4& vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
	w = vec.w;
}

void Vec4::operator=(const Vec4& vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
	w = vec.w;
}

Vec4& Vec4::operator+=(const Vec4& vec)
{
	x += vec.x;
	y += vec.y;
	z += vec.z;
	w += vec.w;

	return *this;
}

Vec4& Vec4::operator-=(const Vec4& vec)
{
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
	w -= vec.w;

	return *this;
}

Vec4& Vec4::operator/=(const Vec4& v2)
{
	x /= v2.x;
	y /= v2.y;
	z /= v2.z;
	w /= v2.w;

	return *this;
}

Vec4& Vec4::operator*=(const Vec4& vec)
{
	x *= vec.x;
	y *= vec.y;
	z *= vec.z;
	w *= vec.w;

	return *this;
}

Vec4& Vec4::operator/=(float scale)
{
	x /= scale;
	y /= scale;
	z /= scale;
	z /= scale;

	return *this;
}

Vec4& Vec4::operator*=(float scale)
{
	x *= scale;
	y *= scale;
	z *= scale;
	w *= scale;

	return *this;
}
