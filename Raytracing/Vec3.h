#pragma once

#include <math.h>
#include <array>

class Vec4;

class Vec3
{
public:

	Vec3() = default;
	Vec3(float x, float y, float z);
	Vec3(const Vec3& vec);

	float x() const { return axis[0]; }
	float y() const { return axis[1]; }
	float z() const { return axis[2]; }

	float r() const { return axis[0]; }
	float g() const { return axis[1]; }
	float b() const { return axis[2]; }

	void operator = (const Vec3& vec);
	inline const Vec3& operator+() const { return *this; }
	inline const Vec3& operator-() const { return Vec3(-axis[0], -axis[0], -axis[0]); }
	Vec3& operator += (const Vec3& vec);
	Vec3& operator -= (const Vec3& vec);
	Vec3& operator /= (const Vec3& vec);
	Vec3& operator *= (const Vec3& vec);
	Vec3& operator /= (float scale);
	Vec3& operator *= (float scale);

	inline float Length() const { return sqrt(axis[0] * axis[0] + axis[1] * axis[1] + axis[2] * axis[2]); }

	static inline float Dot(const Vec3& v1, const Vec3& v2){
		return v1.axis[0] * v2.axis[0] + v1.axis[1] * v2.axis[1] + v1.axis[2] * v2.axis[2];
	}

	static inline Vec3 Cross(const Vec3& v1, const Vec3& v2){
		return Vec3((v1.axis[1] * v2.axis[2] - v1.axis[2] * v2.axis[1]),
			-(v1.axis[0] * v2.axis[2] - v1.axis[2] * v2.axis[0]),
			(v1.axis[0] * v2.axis[1] - v1.axis[1] * v2.axis[0]));
	}

	static inline float Distance(const Vec3& v1, const Vec3& v2) {
		return sqrt((v1.axis[0] - v2.axis[0]) * (v1.axis[0] - v2.axis[0]) + 
			(v1.axis[1] - v2.axis[1]) * (v1.axis[1] - v2.axis[1]) + 
			(v1.axis[2] - v2.axis[2]) * (v1.axis[2] - v2.axis[2]));
	}

	void Normalise();

	std::array<float, 3> axis;
};

inline void Vec3::Normalise()
{
	float k = 1.0f / Length();
	axis[0] *= k;
	axis[1] *= k; 
	axis[2] *= k;

}

inline bool operator==(const Vec3& v1, const Vec3& v2)
{
	return v1.axis[0] == v2.axis[0] && v1.axis[1] == v2.axis[1] && v1.axis[2] == v2.axis[2];
}

inline Vec3 operator*(const Vec3& v1, float scale)
{
	return Vec3(v1.axis[0] * scale, v1.axis[1] * scale, v1.axis[2] * scale);
}

inline Vec3 operator*(float scale, const Vec3& v1)
{
	return Vec3(scale * v1.axis[0], scale * v1.axis[1], scale * v1.axis[2]);
}

inline Vec3 operator*(const Vec3& v1, const Vec3& v2)
{
	return Vec3(v1.axis[0] * v2.axis[0], v1.axis[1] * v2.axis[1], v1.axis[2] * v2.axis[2]);
}

inline Vec3 operator/(Vec3 v1, float scale)
{
	return Vec3(v1.axis[0] / scale, v1.axis[1] / scale, v1.axis[2] / scale);
}

inline Vec3 operator/(const Vec3& v1, const Vec3& v2)
{
	return Vec3(v1.axis[0] / v2.axis[0], v1.axis[1] / v2.axis[1], v1.axis[2] / v2.axis[2]);
}

inline Vec3 operator+(const Vec3& v1, const Vec3& v2)
{
	return Vec3(v1.axis[0] + v2.axis[0], v1.axis[1] + v2.axis[1], v1.axis[2] + v2.axis[2]);
}

inline Vec3 operator-(const Vec3& v1, const Vec3& v2)
{
	return Vec3(v1.axis[0] - v2.axis[0], v1.axis[1] - v2.axis[1], v1.axis[2] - v2.axis[2]);
}

static inline Vec3 Normalise(Vec3 v) { return v / v.Length();}