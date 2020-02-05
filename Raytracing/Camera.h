#pragma once

#include "Vec3.h"
#include "Matrix.h"

#include <vector>

class Camera
{
public:
	Camera() = default;
	Camera(const Vec3& pos) { position = pos; }
	Camera(const Vec3& pos, const Vec3& lookAt);

	inline void SetPos(const Vec3& pos) { position = pos; }
	inline const Vec3& GetPos() { return position; }

	inline void SetLookAt(const Vec3& pos) { lookAtPos = pos; }
	inline const Vec3& GetLookAt() { return lookAtPos; }

	void SetViewMatrix();
	inline const Matrix& GetViewMatrix() { return view; }

private:

	Vec3 position;
	Vec3 lookAtPos;

	Matrix view;
};

