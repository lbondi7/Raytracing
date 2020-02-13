#pragma once

#include "Vec3.h"
#include "Mesh.h"
#include "Triangle.h"

struct Bounds
{

	void Load(const Mesh& mesh);

	void Load(const std::vector<Vertex>& _vertices);

	void Load(const std::vector<Triangle>& _tris);

	void Load(const std::vector<Vec3>& positions);

	void operator=(const Bounds& bb);

	bool RayBoxIntersect(const Ray& r);

	Vec3 max = { 0.0f, 0.0f, 0.0f };
	Vec3 min = { 0.0f, 0.0f, 0.0f };
};

