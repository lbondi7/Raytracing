#pragma once

#include "Vec3.h"

struct Light
{
	Vec3 position;
	Vec3 colour;
	Vec3 size;
	float intensity = 0.0f;
};
