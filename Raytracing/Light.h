#pragma once

#include "Vec3.h"

struct Light
{
	Vec3 position;
	Vec3 colour;

	float intensity = 0.0f;
};
