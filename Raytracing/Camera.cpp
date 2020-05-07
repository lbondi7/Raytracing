#include "Camera.h"

const float M_PI = 3.141592f;

/*

https://www.realtimerendering.com/raytracing/Ray%20Tracing%20in%20a%20Weekend.pdf

author:  Peter Shirley

This function is from ray tracing in one weekend, it is slightly modified to fit my code but most of it is the same.

*/

void Camera::Setup(Vec3 eyePosition, Vec3 lookAt, Vec3 up, float fov, float aspectRatio, float focusDist)
{
    float theta = fov * M_PI / 180;
    float half_height = tan(theta / 2);
    float half_width = aspectRatio * half_height;
    position = eyePosition;
    w = (position - lookAt);
    w.Normalise();
    u = Vec3::Cross(up, w);
    u.Normalise();
    v = Vec3::Cross(w, u);
    lower_left_corner = position - half_width * focusDist * u - half_height * focusDist * v - focusDist * w;
    horizontal = 2 * half_width * focusDist * u;
    vertical = 2 * half_height * focusDist * v;
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
