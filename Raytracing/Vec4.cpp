#include "Vec4.h"

Vec4::Vec4(float x, float y, float z, float w)
{
	this->axis[0] = x;
	this->axis[1] = y;
	this->axis[2] = z;
	this->axis[3] = w;
}

Vec4::Vec4(const Vec4& vec)
{
	axis[0] = vec.axis[0];
	axis[1] = vec.axis[1];
	axis[2] = vec.axis[2];
	axis[3] = vec.axis[3];
}

void Vec4::operator=(const Vec4& vec)
{
	axis[0] = vec.axis[0];
	axis[1] = vec.axis[1];
	axis[2] = vec.axis[2];
	axis[3] = vec.axis[3];
}

Vec4& Vec4::operator+=(const Vec4& vec)
{
	axis[0] += vec.axis[0];
	axis[1] += vec.axis[1];
	axis[2] += vec.axis[2];
	axis[3] += vec.axis[3];

	return *this;
}

Vec4& Vec4::operator-=(const Vec4& vec)
{
	axis[0] -= vec.axis[0];
	axis[1] -= vec.axis[1];
	axis[2] -= vec.axis[2];
	axis[3] -= vec.axis[3];

	return *this;
}

Vec4& Vec4::operator/=(const Vec4& v2)
{
	axis[0] /= v2.axis[0];
	axis[1] /= v2.axis[1];
	axis[2] /= v2.axis[2];
	axis[3] /= v2.axis[3];

	return *this;
}

Vec4& Vec4::operator*=(const Vec4& vec)
{
	axis[0] *= vec.axis[0];
	axis[1] *= vec.axis[1];
	axis[2] *= vec.axis[2];
	axis[3] *= vec.axis[3];

	return *this;
}

Vec4& Vec4::operator/=(float scale)
{
	axis[0] /= scale;
	axis[1] /= scale;
	axis[2] /= scale;
	axis[2] /= scale;

	return *this;
}

Vec4& Vec4::operator*=(float scale)
{
	axis[0] *= scale;
	axis[1] *= scale;
	axis[2] *= scale;
	axis[3] *= scale;

	return *this;
}
