#include "Vec3.h"

Vec3::Vec3(float x, float y, float z)
{
	axis[0] = x;
	axis[1] = y;
	axis[2] = z;
}

Vec3::Vec3(const Vec3& vec)
{
	axis[0] = vec.axis[0];
	axis[1] = vec.axis[1];
	axis[2] = vec.axis[2];
}

Vec3::Vec3(const Vec4& vec)
{
	axis[0] = vec.axis[0];
	axis[1] = vec.axis[1];
	axis[2] = vec.axis[2];

}

void Vec3::operator=(const Vec3& vector)
{
	axis[0] = vector.axis[0];
	axis[1] = vector.axis[1];
	axis[2] = vector.axis[2];

}

Vec3& Vec3::operator+=(const Vec3& vec)
{
	axis[0] += vec.axis[0];
	axis[1] += vec.axis[1];
	axis[2] += vec.axis[2];

	return *this;
}


Vec3& Vec3::operator-=(const Vec3& vec)
{
	axis[0] -= vec.axis[0];
	axis[1] -= vec.axis[1];
	axis[2] -= vec.axis[2];

	return *this;
}

Vec3& Vec3::operator/=(const Vec3& vec)
{
	axis[0] /= vec.axis[0];
	axis[1] /= vec.axis[1];
	axis[2] /= vec.axis[2];

	return *this;
}

Vec3& Vec3::operator*=(const Vec3& vec)
{
	axis[0] *= vec.axis[0];
	axis[1] *= vec.axis[1];
	axis[2] *= vec.axis[2];

	return *this;
}

Vec3& Vec3::operator/=(float scale)
{
	axis[0] /= scale;
	axis[1] /= scale;
	axis[2] /= scale;

	return *this;
}

Vec3& Vec3::operator*=(float scale)
{
	axis[0] *= scale;
	axis[1] *= scale;
	axis[2] *= scale;

	return *this;
}
