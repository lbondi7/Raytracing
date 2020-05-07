#pragma once
#include "Vec3.h"
#include "Vec4.h"
#include "Matrix.h"

#include <iostream>
#include <string>
#include <random>


struct Utilities
{
	static void Print(float value) { std::cout << value << std::endl; }

	static void Print(Vec3 vec) { std::cout << "x: " << vec.axis[0] << "," << " y: " << vec.axis[1] << "," << " z: " << vec.axis[2] << std::endl; }

	static const std::string Vec3ToString(Vec3 vec) { return  "x: " + std::to_string(vec.axis[0]) + ", y: " + std::to_string(vec.axis[1]) + ", z: " + std::to_string(vec.axis[2]); }

	static const std::string Vec3ToString(const std::string name, Vec3 vec) { return  name + ": " + std::to_string(vec.axis[0]) + "," + std::to_string(vec.axis[1]) + "," + std::to_string(vec.axis[2]); }

	static float fPercentageFromMax(float min, float max, float point) { return ((max - point) / (max - min)); }

	static float fPercentageFromMin(float min, float max, float point) { return 1.0f - ((max - point) / (max - min)); }

	static void Translate(const Matrix& m, Vec3& v);

	static void Translate(const Matrix& m, Vec4& v);

	void Scale(const Matrix& m, Vec3& v);

	void Scale(const Matrix& m, Vec4& v);

	static float Randomf(float min, float max);

	static const Vec3& RandomVec3(const Vec3& min, const Vec3& max);

	static std::random_device rd;
};

