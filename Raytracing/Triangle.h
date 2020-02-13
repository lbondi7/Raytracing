#pragma once
#include "Vec4.h"
#include "Vec3.h"
#include "Vertex.h"
#include "Ray.h"

#include <array>

struct Triangle
{
	Triangle(const Vec3& objCenter, const Vertex& v1, const Vertex& v2, const Vertex& v3);
	Triangle(const Vertex& v1, const Vertex& v2, const Vertex& v3);
	Triangle(const Vertex& v1, const Vertex& v2, const Vertex& v3, const Vec3& norm);

	float MinValue(int axis);

	bool Hit(const Ray& ray, float& t, float& u, float& v);

	std::array<Vertex, 3> vertices;
	Vec3 normal;
};
