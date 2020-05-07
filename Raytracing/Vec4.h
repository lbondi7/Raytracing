#pragma once

#include "Vec3.h"

#include <math.h>
#include <array>

class Vec4
{
public:

	Vec4() = default;
	Vec4(float x, float y, float z, float w);
	Vec4(const Vec4& vec);
	Vec4(const Vec3& vec);

	static Vec4 TranslationVector(const Vec3& vec);
	static Vec4 RotationVector(const Vec3& vec);
	
	float x() const { return axis[0]; }
	float y() const { return axis[1]; }
	float z() const { return axis[2]; }
	float w() const { return axis[3]; }

	float r() const { return axis[0]; }
	float g() const { return axis[1]; }
	float b() const { return axis[2]; }
	float a() const { return axis[3]; }

	void operator = (const Vec4& vec);
	inline const Vec4& operator+() const { return *this; }
	inline const Vec4& operator-() const { return Vec4(-axis[0], -axis[1], -axis[2], -axis[3]); }
	Vec4& operator += (const Vec4& vec);
	Vec4& operator -= (const Vec4& vec);
	Vec4& operator /= (const Vec4& vec);
	Vec4& operator *= (const Vec4& vec);
	Vec4& operator /= (float scale);
	Vec4& operator *= (float scale);

	//inline Vec3 xyz() { return Vec3(this->x, this->y, this->z); }

	inline float Length() const { return sqrt(axis[0] * axis[0] + axis[1] * axis[1] + axis[2] * axis[2] + axis[3] * axis[3]); }

	static inline float Dot(const Vec4& v1, const Vec4& v2) {
		return v1.axis[0] * v2.axis[0] + v1.axis[1] * v2.axis[1] + v1.axis[2] * v2.axis[2] + v1.axis[3] * v2.axis[3];
	}

	std::array<float, 4> axis{0.0f, 0.0f, 0.0f, 0.0f};

	void Normalise();
};

inline void Vec4::Normalise()
{
	float k = 1.0 / sqrt(axis[0] * axis[0] + axis[1] * axis[1] + axis[2] * axis[2] + axis[3] * axis[3]);

	axis[0] *= k;
	axis[1] *= k;
	axis[2] *= k;
	axis[3] *= k;
}

inline bool operator==(const Vec4& v1, const Vec4& v2)
{
	return v1.axis[0] == v2.axis[0] && v1.axis[1] == v2.axis[1] && v1.axis[2] == v2.axis[2] && v1.axis[3] == v2.axis[3];
}

inline Vec4 operator*(const Vec4& v1, float scale)
{
	return Vec4(scale * v1.axis[0], scale * v1.axis[1], scale * v1.axis[2], scale * v1.axis[3]);
}

inline Vec4 operator*(float scale, const Vec4& v1)
{
	return Vec4(scale * v1.axis[0], scale * v1.axis[1], scale * v1.axis[2], scale * v1.axis[3]);
}

inline Vec4 operator*(const Vec4& v1, const Vec4& v2)
{
	return Vec4(v1.axis[0] * v2.axis[0], v1.axis[1] * v2.axis[1], v1.axis[2] * v2.axis[2], v1.axis[3] * v2.axis[3]);
}

inline Vec4 operator/(const Vec4& v1, float scale)
{
	return Vec4(v1.axis[0] / scale, v1.axis[1] / scale, v1.axis[2] / scale, v1.axis[3] / scale);
}

inline Vec4 operator/(const Vec4& v1, const Vec4& v2)
{
	return Vec4(v1.axis[0] / v2.axis[0], v1.axis[1] / v2.axis[1], v1.axis[2] / v2.axis[2], v1.axis[3] / v2.axis[3]);
}

inline Vec4 operator+(const Vec4& v1, const Vec4& v2)
{
	return Vec4(v1.axis[0] + v2.axis[0], v1.axis[1] + v2.axis[1], v1.axis[2] + v2.axis[2], v1.axis[3] + v2.axis[3]);
}

inline Vec4 operator-(const Vec4& v1, const Vec4& v2)
{
	return Vec4(v1.axis[0] - v2.axis[0], v1.axis[1] - v2.axis[1], v1.axis[2] - v2.axis[2], v1.axis[3] - v2.axis[3]);
}

inline Vec4 Normalise(Vec4 vec)
{
	return vec / vec.Length();
}