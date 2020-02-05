#pragma once

#include "Ray.h"
#include "Bounds.h"

class Object
{
public:
	Object() = default;
	Object(Vec3 _center);
	~Object();

	void Load(const std::string& mesh);

	Vec3 HitObject(const Ray& ray);

	bool HitTriangle(const Ray& ray, const Vec3& v0, const Vec3& v1, const Vec3& v2, float& t, float& u, float& v);

	Mesh mesh;

	Vec3 center = { 0.0f, 0.0f, 0.0f };

	Bounds boundingBox;
};

