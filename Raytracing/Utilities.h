#pragma once
#include "Vec3.h"

#include <iostream>


struct Utilities
{

	static void Print(float value) { std::cout << value << std::endl; }

	static void Print(Vec3 vec) { std::cout << "x: " << vec.axis[0] << "," << " y: " << vec.axis[1] << "," << " z: " << vec.axis[2] << std::endl; }

	static float fPercentageFromMax(float min, float max, float point) { return ((max - point) / (max - min)); }

	static float fPercentageFromMin(float min, float max, float point) { return 1.0f - ((max - point) / (max - min)); }


};

