#pragma once

#include "Ray.h"
#include "Bounds.h"
#include "Triangle.h"
#include "HitRecord.h"
#include "Transform.h"
#include "Matrix.h"

class Object
{
public:
	Object() = default;
	Object(Vec3 _center);
	~Object();

	void Load(const std::string& mesh);

	void Load(const std::string& mesh, const Vec3& _center);

	void Load(const std::string& mesh, const Vec3& _center, int _id);

	void Load(const std::string& meshType, const Transform& _tran, int _id);

	void SetCenter(Vec3 _center);

	void Update();

	Mesh mesh;

	std::vector<Triangle> tris;

	Vec3 center = { 0.0f, 0.0f, 0.0f };

	Bounds boundingBox;

	bool isLight = false;
	int id = 0;
	Matrix objectMatrix;

	Transform transform;

	bool update = false;
};

