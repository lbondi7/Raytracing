#include "Vec3.h"

Vec3::Vec3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vec3::Vec3(const Vec3& vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
}

void Vec3::operator=(const Vec3& vector)
{
	x = vector.x;
	y = vector.y;
	z = vector.z;
}

Vec3& Vec3::operator+=(const Vec3& v2)
{
	x += v2.x;
	y += v2.y;
	z += v2.z;

	return *this;
}


Vec3& Vec3::operator-=(const Vec3& v2)
{
	x -= v2.x;
	y -= v2.y;
	z -= v2.z;

	return *this;
}

Vec3& Vec3::operator/=(const Vec3& v2)
{
	x /= v2.x;
	y /= v2.y;
	z /= v2.z;

	return *this;
}

Vec3& Vec3::operator*=(const Vec3& v2)
{
	x *= v2.x;
	y *= v2.y;
	z *= v2.z;

	return *this;
}

Vec3& Vec3::operator/=(float scale)
{
	x /= scale;
	y /= scale;
	z /= scale;

	return *this;
}

Vec3& Vec3::operator*=(float scale)
{
	x *= scale;
	y *= scale;
	z *= scale;

	return *this;
}
