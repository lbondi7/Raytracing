#include "Camera.h"

Camera::Camera(const Vec3& pos, const Vec3& lookAt)
{
	position = pos;
	lookAtPos = lookAt;
}

void Camera::SetViewMatrix()
{
    Vec3 forward = Vec3::Normalise(position - lookAtPos);
    Vec3 right = Vec3::Cross(Vec3::Normalise({0, 1, 0}), forward);
    Vec3 up = Vec3::Cross(forward, right);


    view.data[0][0] = right.x;
    view.data[0][1] = right.y;
    view.data[0][2] = right.z;
    view.data[1][0] = up.x;
    view.data[1][1] = up.y;
    view.data[1][2] = up.z;
    view.data[2][0] = forward.x;
    view.data[2][1] = forward.y;
    view.data[2][2] = forward.z;

    view.data[3][0] = position.x;
    view.data[3][1] = position.y;
    view.data[3][2] = position.z;
}
