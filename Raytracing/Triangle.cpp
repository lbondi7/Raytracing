#include "Triangle.h"
#include "Constants.h"

Triangle::Triangle(const Vec3& objCenter, const Vertex& v1, const Vertex& v2, const Vertex& v3)
{
	vertices[0] = v1;
	vertices[1] = v2;
	vertices[2] = v3;

	for (size_t i = 0; i < 3; ++i)
	{
		vertices[i].pos += objCenter;
	}

    for (size_t i = 0; i < 3; ++i)
    {
        minPoints[i] = std::min(std::min(vertices[0].pos.axis[i], vertices[1].pos.axis[i]), vertices[2].pos.axis[i]);
    }

	Vec3 u = v2.pos - v1.pos;
	Vec3 v = v3.pos - v1.pos;

	normal = Vec3::Cross(u, v);
}

Triangle::Triangle(Vec3* objCenter, const Vertex& v1, const Vertex& v2, const Vertex& v3)
{
    vertices[0] = v1;
    vertices[1] = v2;
    vertices[2] = v3;

    objectCenter = objCenter;

    for (size_t i = 0; i < 3; ++i)
    {
        //vertPosWorld[i] = vertices[i].pos + *objectCenter;
        vertices[i].pos = vertices[i].localPos + *objCenter;
    }

    for (size_t i = 0; i < 3; ++i)
    {
        //minPoints[i] = std::min(std::min(vertPosWorld[0].axis[i], vertPosWorld[1].axis[i]), vertPosWorld[2].axis[i]);
        minPoints[i] = std::min(std::min(vertices[0].pos.axis[i], vertices[1].pos.axis[i]), vertices[2].pos.axis[i]);
    }

    edges[0] = vertices[1].pos - vertices[0].pos;
    edges[1] = vertices[2].pos - vertices[1].pos;
    edges[2] = vertices[0].pos - vertices[2].pos;

    Vec3 u = vertices[1].pos - vertices[0].pos;
    Vec3 v = vertices[2].pos - vertices[0].pos;

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

Triangle::~Triangle()
{
    
}

float Triangle::MinValue(int axis)
{
    return minPoints[axis];
 //   return std::min(std::min(vertices[0].pos.axis[axis], vertices[1].pos.axis[axis]), vertices[2].pos.axis[axis]);
}

bool Triangle::Hit(
    const Ray& ray,
    float& t, float& u, float& v)
{
    // no need to normalize
    float denom = Vec3::Dot(normal, normal);

   // float area2 = normal.Length();

    // Step 1: finding P

    // check if ray and plane are parallel ?
    float NdotRayDirection = Vec3::Dot(normal, ray.direction);
    if (fabs(NdotRayDirection) < kEPSILON) // almost 0 
        return false; // they are parallel so they don't intersect ! 

    // compute d parameter using equation 2
    float d = Vec3::Dot(normal, vertices[0].pos);

    // compute t (equation 3)
    t = (Vec3::Dot(normal, ray.origin) + d) / NdotRayDirection;
    // check if the triangle is in behind the ray
    if (t < 0) return false; // the triangle is behind 

    // compute the intersection point using equation 1
    Vec3 P = ray.origin + t * ray.direction;

    Vec3 C; // vector perpendicular to triangle's plane 

    Vec3 vp0 = P - vertices[0].pos;
    C = Vec3::Cross(edges[0], vp0);
    if (Vec3::Dot(normal, C) < 0) return false; // P is on the right side 

    Vec3 vp1 = P - vertices[1].pos;
    C = Vec3::Cross(edges[1], vp1);
    if ((u = Vec3::Dot(normal, C)) < 0)  return false; // P is on the right side 

    Vec3 vp2 = P - vertices[2].pos;
    C = Vec3::Cross(edges[2], vp2);
    if ((v = Vec3::Dot(normal, C)) < 0) return false; // P is on the right side; 

    u /= denom;
    v /= denom;

    return true; // this ray hits the triangle 
}

void Triangle::Update()
{
    for (size_t i = 0; i < 3; ++i)
    {
        //vertPosWorld[i] = vertices[i].pos + *objectCenter;
        vertices[i].pos = vertices[i].localPos + *objectCenter;
    }

    for (size_t i = 0; i < 3; ++i)
    {
        //minPoints[i] = std::min(std::min(vertPosWorld[0].axis[i], vertPosWorld[1].axis[i]), vertPosWorld[2].axis[i]);
        minPoints[i] = std::min(std::min(vertices[0].pos.axis[i], vertices[1].pos.axis[i]), vertices[2].pos.axis[i]);
    }

    edges[0] = vertices[1].pos - vertices[0].pos;
    edges[1] = vertices[2].pos - vertices[1].pos;
    edges[2] = vertices[0].pos - vertices[2].pos;

    Vec3 u = vertices[1].pos - vertices[0].pos;
    Vec3 v = vertices[2].pos - vertices[0].pos;

    normal = Vec3::Cross(u, v);
}
