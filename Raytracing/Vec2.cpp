#include "Vec2.h"

Vec2::Vec2(float x, float y)
{
	this->x = x;
	this->y = y;
}

Vec2::Vec2(const Vec2& vec)
{
	x = vec.x;
	y = vec.y;
}

void Vec2::operator=(const Vec2& vec)
{
	x = vec.x;
	y = vec.y;
}

Vec2& Vec2::operator+=(const Vec2& vec)
{
	x += vec.x;
	y += vec.y;

	return *this;
}

Vec2& Vec2::operator-=(const Vec2& vec)
{
	x -= vec.x;
	y -= vec.y;

	return *this;
}

Vec2& Vec2::operator/=(const Vec2& vec)
{
	x /= vec.x;
	y /= vec.y;

	return *this;
}

Vec2& Vec2::operator*=(const Vec2& vec)
{
	x *= vec.x;
	y *= vec.y;

	return *this;
}

Vec2& Vec2::operator/=(float scale)
{
	x /= scale;
	y /= scale;

	return *this;
}

Vec2& Vec2::operator*=(float scale)
{
	x *= scale;
	y *= scale;

	return *this;
}
