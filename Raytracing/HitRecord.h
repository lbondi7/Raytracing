#pragma once

#include "Vec3.h"

struct HitRecord
{
	float t;
	Vec3 colour;
	Vec3 normal;
	Vec3 point;
	int obj_id = 0;
	int tri_id = 0;
};
