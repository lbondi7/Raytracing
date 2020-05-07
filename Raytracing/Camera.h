#pragma once

#include "Vec3.h"
#include "Matrix.h"

#include <vector>

class Camera
{
public:
	Camera() = default;

	void Setup(Vec3 _origin, Vec3 lookAt, Vec3 up, float fov, float aspectRatio, float focus_dist);

	inline void SetLookAt(const Vec3& pos) { lookAtPos = pos; }
	inline const Vec3& GetLookAt() { return lookAtPos; }

	void SetViewMatrix();
	inline const Matrix& GetViewMatrix() { return view; }

    //ray get_ray(float s, float t) {
    //    vec3 rd = lens_radius * random_in_unit_disk();
    //    vec3 offset = u * rd.x() + v * rd.y();
    //    return ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset);
    //}

    Vec3 position;
    Vec3 lower_left_corner;
    Vec3 horizontal;
    Vec3 vertical;
	Vec3 lookAtPos;
	Vec3 u, v, w;
	Matrix view;
};

