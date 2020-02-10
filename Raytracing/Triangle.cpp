#include "Triangle.h"

constexpr float kEpsilon = 1e-8;

Triangle::Triangle(const Vec3& objCenter, const Vertex& v1, const Vertex& v2, const Vertex& v3)
{
	vertices[0] = v1;
	vertices[1] = v2;
	vertices[2] = v3;

	for (size_t i = 0; i < 3; ++i)
	{
		vertices[i].pos += objCenter;
	}

	Vec3 u = v2.pos - v1.pos;
	Vec3 v = v3.pos - v1.pos;

	normal = Vec3::Cross(u, v);
}

Triangle::Triangle(const Vertex& v1, const Vertex& v2, const Vertex& v3)
{
	vertices[0] = v1;
	vertices[1] = v2;
	vertices[2] = v3;

	Vec3 u = v2.pos - v1.pos;
	Vec3 v = v3.pos - v1.pos;

	normal = Vec3::Cross(u, v);
}

Triangle::Triangle(const Vertex& v1, const Vertex& v2, const Vertex& v3, const Vec3& norm)
{
	vertices[0] = v1;
	vertices[1] = v2;
	vertices[2] = v3;

	normal = norm;
}

float Triangle::MinValue(int axis)
{
	return std::min(std::min(vertices[0].pos.axis[axis], vertices[1].pos.axis[axis]), vertices[2].pos.axis[axis]);
}

bool Triangle::Hit(
    const Ray& ray,
    float& t, float& u, float& v)
{
    // compute plane's normal
    Vec3 v0v1 = vertices[1].pos - vertices[0].pos;
    Vec3 v0v2 = vertices[2].pos - vertices[0].pos;
    // no need to normalize
    Vec3 N = Vec3::Cross(v0v1, v0v2); // N 
    float denom = Vec3::Dot(N, N);

    float area2 = N.Length();

    // Step 1: finding P

    // check if ray and plane are parallel ?
    float NdotRayDirection = Vec3::Dot(N, ray.direction);
    if (fabs(NdotRayDirection) < kEpsilon) // almost 0 
        return false; // they are parallel so they don't intersect ! 

    // compute d parameter using equation 2
    float d = Vec3::Dot(N, vertices[0].pos);

    // compute t (equation 3)
    t = (Vec3::Dot(N, ray.origin) + d) / NdotRayDirection;
    // check if the triangle is in behind the ray
    if (t < 0) return false; // the triangle is behind 

    // compute the intersection point using equation 1
    Vec3 P = ray.origin + t * ray.direction;

    // Step 2: inside-outside test
    Vec3 C; // vector perpendicular to triangle's plane 

    // edge 0
    Vec3 edge0 = vertices[1].pos - vertices[0].pos;
    Vec3 vp0 = P - vertices[0].pos;
    C = Vec3::Cross(edge0, vp0);
    if (Vec3::Dot(N, C) < 0) return false; // P is on the right side 

    // edge 1
    Vec3 edge1 = vertices[2].pos - vertices[1].pos;
    Vec3 vp1 = P - vertices[1].pos;
    C = Vec3::Cross(edge1, vp1);
    if ((u = Vec3::Dot(N, C)) < 0)  return false; // P is on the right side 

    // edge 2
    Vec3 edge2 = vertices[0].pos - vertices[2].pos;
    Vec3 vp2 = P - vertices[2].pos;
    C = Vec3::Cross(edge2, vp2);
    if ((v = Vec3::Dot(N, C)) < 0) return false; // P is on the right side; 

    u /= denom;
    v /= denom;

    return true; // this ray hits the triangle 
}