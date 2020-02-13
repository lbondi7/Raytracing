#include "Camera.h"

Camera::Camera(const Vec3& pos, const Vec3& lookAt)
{
	position = pos;
	lookAtPos = lookAt;
}

void Camera::SetViewMatrix()
{
    Vec3 forward = Normalise(position - lookAtPos);
    Vec3 right = Vec3::Cross(Normalise({0, 1, 0}), forward);
    Vec3 up = Vec3::Cross(forward, right);


    view.data[0][0] = right.axis[0];
    view.data[0][1] = right.axis[1];
    view.data[0][2] = right.axis[2];
    view.data[1][0] = up.axis[0];
    view.data[1][1] = up.axis[1];
    view.data[1][2] = up.axis[2];
    view.data[2][0] = forward.axis[0];
    view.data[2][1] = forward.axis[1];
    view.data[2][2] = forward.axis[2];

    view.data[3][0] = position.axis[0];
    view.data[3][1] = position.axis[1];
    view.data[3][2] = position.axis[2];
}
