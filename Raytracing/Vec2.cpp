#include "Vec2.h"

Vec2::Vec2(float x, float y)
{
	this->axis[0] = x;
	this->axis[1] = y;
}

Vec2::Vec2(const Vec2& vec)
{
	axis[0] = vec.axis[0];
	axis[1] = vec.axis[1];
}

void Vec2::operator=(const Vec2& vec)
{
	axis[0] = vec.axis[0];
	axis[1] = vec.axis[1];
}

Vec2& Vec2::operator+=(const Vec2& vec)
{
	axis[0] += vec.axis[0];
	axis[1] += vec.axis[1];

	return *this;
}

Vec2& Vec2::operator-=(const Vec2& vec)
{
	axis[0] -= vec.axis[0];
	axis[1] -= vec.axis[1];

	return *this;
}

Vec2& Vec2::operator/=(const Vec2& vec)
{
	axis[0] /= vec.axis[0];
	axis[1] /= vec.axis[1];

	return *this;
}

Vec2& Vec2::operator*=(const Vec2& vec)
{
	axis[0] *= vec.axis[0];
	axis[1] *= vec.axis[1];

	return *this;
}

Vec2& Vec2::operator/=(float scale)
{
	axis[0] /= scale;
	axis[1] /= scale;

	return *this;
}

Vec2& Vec2::operator*=(float scale)
{
	axis[0] *= scale;
	axis[1] *= scale;

	return *this;
}
