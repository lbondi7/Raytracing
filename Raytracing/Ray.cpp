#include "Ray.h"

Ray::Ray(const Vec3& origin, const Vec3& direction)
{
	this->origin = origin;
	this->direction = direction;
	this->direction.Normalise();
	invDirection = Vec3(1.0f / direction.axis[0], 1.0f / direction.axis[1], 1.0f / direction.axis[2]);

	for (size_t i = 0; i < 3; ++i)
	{
		sign[i] = (invDirection.axis[i] < 0);
	}
}

void Ray::operator=(const Ray& ray)
{
	origin = ray.origin;
	direction = ray.direction;
	invDirection = invDirection;
}
