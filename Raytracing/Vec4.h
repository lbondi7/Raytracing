#pragma once

#include <math.h>

class Vec4
{
public:

	Vec4() = default;
	Vec4(float x, float y, float z, float w);
	Vec4(const Vec4& vec);

	void operator = (const Vec4& vec);
	inline const Vec4& operator+() const { return *this; }
	inline const Vec4& operator-() const { return Vec4(-x, -y, -z, -w); }
	Vec4& operator += (const Vec4& vec);
	Vec4& operator -= (const Vec4& vec);
	Vec4& operator /= (const Vec4& vec);
	Vec4& operator *= (const Vec4& vec);
	Vec4& operator /= (float scale);
	Vec4& operator *= (float scale);

	inline float Length() const { return sqrt(x * x + y * y + z * z + w * w); }

	static inline float Dot(const Vec4& v1, const Vec4& v2) {
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
	}

	float x = 0;
	float y = 0;
	float z = 0;
	float w = 0;

	void MakeUnitVector();
};

inline void Vec4::MakeUnitVector()
{
	float k = 1.0 / sqrt(x * x + y * y + z * z + w * w);

	x *= k;
	y *= k;
	z *= k;
	w *= k;
}

inline bool operator==(const Vec4& v1, const Vec4& v2)
{
	return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z && v1.w == v2.w;
}

inline Vec4 operator*(const Vec4& v1, float scale)
{
	return Vec4(scale * v1.x, scale * v1.y, scale * v1.z, scale * v1.w);
}

inline Vec4 operator*(float scale, const Vec4& v1)
{
	return Vec4(scale * v1.x, scale * v1.y, scale * v1.z, scale * v1.w);
}

inline Vec4 operator*(const Vec4& v1, const Vec4& v2)
{
	return Vec4(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w);
}

inline Vec4 operator/(const Vec4& v1, float scale)
{
	return Vec4(v1.x / scale, v1.y / scale, v1.z / scale, v1.w / scale);
}

inline Vec4 operator/(const Vec4& v1, const Vec4& v2)
{
	return Vec4(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.w / v2.w);
}

inline Vec4 operator+(const Vec4& v1, const Vec4& v2)
{
	return Vec4(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w);
}

inline Vec4 operator-(const Vec4& v1, const Vec4& v2)
{
	return Vec4(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w);
}

inline Vec4 UnitVector(Vec4 vec)
{
	return vec / vec.Length();
}