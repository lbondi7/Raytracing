#pragma once

#include "Vec3.h"

struct HitRecord
{
	float t;
	Vec3 colour;
	Vec3 normal;
	Vec3 point;
};
