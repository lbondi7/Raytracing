#include "Ray.h"

Ray::Ray(const Vec3& origin, const Vec3& direction)
{
	this->origin = origin;
	this->direction = direction;
	invDirection = Vec3(1 / direction.axis[0], 1 / direction.axis[2],1 / direction.axis[2]);

	for (size_t i = 0; i < 3; ++i)
	{
		sign[i] = (invDirection.axis[i] < 0);
	}
}
