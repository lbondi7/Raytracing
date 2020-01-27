#pragma once
#include <math.h>

class Vec3
{
public:

	Vec3() = default;
	Vec3(float x, float y, float z);
	Vec3(const Vec3& vec);

	void operator = (const Vec3& vector);
	inline const Vec3& operator+() const { return *this; }
	inline const Vec3& operator-() const { return Vec3(-x, -y, -z); }
	Vec3& operator += (const Vec3& v2);
	Vec3& operator -= (const Vec3& v2);
	Vec3& operator /= (const Vec3& v2);
	Vec3& operator *= (const Vec3& v2);
	Vec3& operator /= (float scale);
	Vec3& operator *= (float scale);

	inline float Length() const { return sqrt(x * x + y * y + z * z); }

	static inline float Dot(const Vec3& v1, const Vec3& v2){
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	static inline Vec3 Cross(const Vec3& v1, const Vec3& v2){
		return Vec3((v1.y * v2.z - v1.z * v2.y),
			-(v1.x * v2.z - v1.z * v2.x),
			(v1.x * v2.y - v1.y * v2.x));
	}

	float x = 0;
	float y = 0;
	float z = 0;

	void MakeUnitVector();



};

inline void Vec3::MakeUnitVector()
{
	float k = 1.0 / sqrt(x * x + y * y + z * z);
	x *= k;
	y *= k; 
	z *= k;
}

inline Vec3 operator*(const Vec3& v1, float scale)
{
	return Vec3(scale * v1.x, scale * v1.y, scale * v1.z);
}

inline Vec3 operator*(float scale, const Vec3& v1)
{
	return Vec3(scale * v1.x, scale * v1.y, scale * v1.z);
}

inline Vec3 operator*(const Vec3& v1, const Vec3& v2)
{
	return Vec3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

inline Vec3 operator/(const Vec3& v1, float scale)
{
	return Vec3(v1.x / scale, v1.y / scale, v1.z / scale);
}

inline Vec3 operator/(const Vec3& v1, const Vec3& v2)
{
	return Vec3(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
}

inline Vec3 operator+(const Vec3& v1, const Vec3& v2)
{
	return Vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

inline Vec3 operator-(const Vec3& v1, const Vec3& v2)
{
	return Vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

inline Vec3 UnitVector(Vec3 v)
{
	return v / v.Length();
}