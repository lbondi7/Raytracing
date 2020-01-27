#pragma once
#include "Vec3.h"

#include <iostream>


struct Utilities
{

	static void Print(float value) { std::cout << value << std::endl; }

	static void Print(Vec3 vec) { std::cout << "x: " << vec.x << "," << " y: " << vec.y << "," << " z: " << vec.z << std::endl; }

};

