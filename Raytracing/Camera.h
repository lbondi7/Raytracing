#pragma once

#include "Vec3.h"
#include <vector>

class Camera
{
public:
	Camera() = default;
	Camera(const Vec3& pos) { position = pos; }
	Camera(const Vec3& pos, const Vec3& dir);

	inline void SetPos(const Vec3& pos) { position = pos; }
	inline const Vec3& GetPos() { return position; }

	inline void SetDir(const Vec3& pos) { position = pos; }
	inline const Vec3& GetDir() { return position; }


private:

	Vec3 position;
	Vec3 direction;


};

