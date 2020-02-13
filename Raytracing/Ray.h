#pragma once

#include "Vec3.h"

class Ray
{

public:
	Ray() = default;
	Ray(const Vec3& origin, const Vec3& direction);
	//Vec3 Origin() const { return origin; }
	//Vec3 Direction() const { return direction; }
	Vec3 PointAtParameter(float t) const { return origin + direction * t; }

	Vec3 origin;
	Vec3 direction;
	Vec3 invDirection;
	int sign[3];
};

