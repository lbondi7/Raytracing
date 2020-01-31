#pragma once

#include <math.h>

class Vec2
{
public:

	Vec2() = default;
	Vec2(float x, float y);
	Vec2(const Vec2& vec);

	void operator = (const Vec2& vec);
	inline const Vec2& operator+() const { return *this; }
	inline const Vec2& operator-() const { return Vec2(-x, -y); }
	Vec2& operator += (const Vec2& vec);
	Vec2& operator -= (const Vec2& vec);
	Vec2& operator /= (const Vec2& vec);
	Vec2& operator *= (const Vec2& vec);
	Vec2& operator /= (float scale);
	Vec2& operator *= (float scale);

	inline float Length() const { return sqrt(x * x + y * y); }

	static inline float Dot(const Vec2& v1, const Vec2& v2) {
		return v1.x * v2.x + v1.y * v2.y;
	}

	float x = 0;
	float y = 0;

	void MakeUnitVector();
};

inline void Vec2::MakeUnitVector()
{
	float k = 1.0 / sqrt(x * x + y * y);
	x *= k;
	y *= k;
}

inline bool operator==(const Vec2& v1, const Vec2& v2)
{
	return v1.x == v2.x && v1.y == v2.y;
}

inline Vec2 operator*(const Vec2& v1, float scale)
{
	return Vec2(scale * v1.x, scale * v1.y);
}

inline Vec2 operator*(float scale, const Vec2& v1)
{
	return Vec2(scale * v1.x, scale * v1.y);
}

inline Vec2 operator*(const Vec2& v1, const Vec2& v2)
{
	return Vec2(v1.x * v2.x, v1.y * v2.y);
}

inline Vec2 operator/(const Vec2& v1, float scale)
{
	return Vec2(v1.x / scale, v1.y / scale);
}

inline Vec2 operator/(const Vec2& v1, const Vec2& v2)
{
	return Vec2(v1.x / v2.x, v1.y / v2.y);
}

inline Vec2 operator+(const Vec2& v1, const Vec2& v2)
{
	return Vec2(v1.x + v2.x, v1.y + v2.y);
}

inline Vec2 operator-(const Vec2& v1, const Vec2& v2)
{
	return Vec2(v1.x - v2.x, v1.y - v2.y);
}

inline Vec2 UnitVector(Vec2 v)
{
	return v / v.Length();
}