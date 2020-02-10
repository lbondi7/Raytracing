#pragma once

#include <math.h>
#include <array>

class Vec2
{
public:

	Vec2() = default;
	Vec2(float x, float y);
	Vec2(const Vec2& vec);

	void operator = (const Vec2& vec);
	inline const Vec2& operator+() const { return *this; }
	inline const Vec2& operator-() const { return Vec2(-axis[0], -axis[1]); }
	Vec2& operator += (const Vec2& vec);
	Vec2& operator -= (const Vec2& vec);
	Vec2& operator /= (const Vec2& vec);
	Vec2& operator *= (const Vec2& vec);
	Vec2& operator /= (float scale);
	Vec2& operator *= (float scale);

	inline float Length() const { return sqrt(axis[0] * axis[0] + axis[1] * axis[1]); }

	static inline float Dot(const Vec2& v1, const Vec2& v2) {
		return v1.axis[0] * v2.axis[0] + v1.axis[1] * v2.axis[1];
	}

	float x = 0;
	float y = 0;

	std::array<float, 2> axis;

	void MakeUnitVector();
};

inline void Vec2::MakeUnitVector()
{
	float k = 1.0 / Length();
	axis[0] *= k;
	axis[1] *= k;
}

inline bool operator==(const Vec2& v1, const Vec2& v2)
{
	return v1.axis[0] == v2.axis[0] && v1.axis[1] == v2.axis[1];
}

inline Vec2 operator*(const Vec2& v1, float scale)
{
	return Vec2(scale * v1.axis[0], scale * v1.axis[1]);
}

inline Vec2 operator*(float scale, const Vec2& v1)
{
	return Vec2(scale * v1.axis[0], scale * v1.axis[1]);
}

inline Vec2 operator*(const Vec2& v1, const Vec2& v2)
{
	return Vec2(v1.axis[0] * v2.axis[0], v1.axis[1] * v2.axis[1]);
}

inline Vec2 operator/(const Vec2& v1, float scale)
{
	return Vec2(v1.axis[0] / scale, v1.axis[1] / scale);
}

inline Vec2 operator/(const Vec2& v1, const Vec2& v2)
{
	return Vec2(v1.axis[0] / v2.axis[0], v1.axis[1] / v2.axis[1]);
}

inline Vec2 operator+(const Vec2& v1, const Vec2& v2)
{
	return Vec2(v1.axis[0] + v2.axis[0], v1.axis[1] + v2.axis[1]);
}

inline Vec2 operator-(const Vec2& v1, const Vec2& v2)
{
	return Vec2(v1.axis[0] - v2.axis[0], v1.axis[1] - v2.axis[1]);
}

inline Vec2 UnitVector(Vec2 v)
{
	return v / v.Length();
}