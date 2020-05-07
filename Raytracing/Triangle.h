#pragma once
#include "Vec4.h"
#include "Vec3.h"
#include "Vertex.h"
#include "Ray.h"
#include "Image.h"

#include <array>

struct Triangle
{
	Triangle() = default;
	Triangle(const Vec3& objCenter, const Vertex& v1, const Vertex& v2, const Vertex& v3);
	Triangle(Vec3* objCenter, const Vertex& v1, const Vertex& v2, const Vertex& v3);
	Triangle(const Vertex& v1, const Vertex& v2, const Vertex& v3);
	Triangle(const Vertex& v1, const Vertex& v2, const Vertex& v3, const Vec3& norm);

	~Triangle();

	float MinValue(int axis);

	bool Hit(const Ray& ray, float& t, float& u, float& v);

	bool MTHit(Ray& ray, float& t, float& u, float& v);

	bool ShadowMTHit(Ray& ray, float& t, float& u, float& v);

	bool LewisHit(Ray& ray, float& t, float& u, float& v);

	void Update();

	std::array<Vertex, 3> vertices;
	Vec3 normal;
	Vec3 normalNN;
	std::array<float, 3> minPoints;
	std::array<Vec3, 3> edges;
	Vec3 otherEdge;
	int tri_id = 0;
	int obj_id = 0;

	Vec3* objectCenter = nullptr;
	Image* image = nullptr;
	bool isLight = false;
};

