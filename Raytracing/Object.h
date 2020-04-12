#pragma once

#include "Ray.h"
#include "Bounds.h"
#include "Triangle.h"
#include "HitRecord.h"
#include "Matrix.h"

class Object
{
public:
	Object() = default;
	Object(Vec3 _center);
	~Object();

	void Load(const std::string& mesh);

	void Load(const std::string& mesh, const Vec3& _center);

	Vec3 HitObject(const Ray& ray);

	bool Hit(const Ray& ray, Vec3& colour);

	bool Hit(const Ray& ray, HitRecord& rec);

	void Hit(const Ray& ray, std::vector<HitRecord>& hits);

	bool HitTriangle(const Ray& ray, const Vec3& v0, const Vec3& v1, const Vec3& v2, float& t, float& u, float& v);

	bool HitTriangle(const Ray& ray, const Vec3& v0, const Vec3& v1, const Vec3& v2, float& t, float& u, float& v, HitRecord& rec);

	void SetCenter(Vec3 _center);

	void Update();

	Mesh mesh;

	std::vector<Triangle> tris;

	Vec3 center = { 0.0f, 0.0f, 0.0f };

	Bounds boundingBox;

	bool isLight = false;

	Matrix objectMatrix;
};

