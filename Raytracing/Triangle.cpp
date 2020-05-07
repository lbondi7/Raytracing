#include "Triangle.h"
#include "Constants.h"
#include "DebugPrinter.h"
#include "Utilities.h"

#define CULLING 1;

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
    //for (size_t i = 0; i < 3; i++)
    //{
    //    if ((int)normal.axis[i] == -0)
    //    {
    //        normal.axis[i] = 0.0;
    //    }
    //}
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

    //Vec3 u = vertices[1].pos - vertices[0].pos;
    otherEdge = vertices[2].pos - vertices[0].pos;

    normal = Vec3::Cross(edges[0], otherEdge);
    normalNN = normal;
    normal.Normalise();
    //for (size_t i = 0; i < 3; i++)
    //{
    //    if (normal.axis[i] == -0.0)
    //        normal.axis[i] = 0.0;
    //}
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

    Vec3 vp0 = P - vertices[0].pos;
    Vec3 C = Vec3::Cross(edges[0], vp0);
    if (Vec3::Dot(normal, C) < 0.0f) return false; // P is on the right side 

    Vec3 vp1 = P - vertices[1].pos;
    C = Vec3::Cross(edges[1], vp1);
    if ((u = Vec3::Dot(normal, C)) < 0.0f)  return false; // P is on the right side 

    Vec3 vp2 = P - vertices[2].pos;
    C = Vec3::Cross(edges[2], vp2);
    if ((v = Vec3::Dot(normal, C)) < 0.0f) return false; // P is on the right side; 

    // no need to normalize
    float denom = Vec3::Dot(normal, normal);

    u /= denom;
    v /= denom;

    return true; // this ray hits the triangle 
}

/*
https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/moller-trumbore-ray-triangle-intersection

author: Tomas Akenine-Möller

The ray triangle functions are from the link above. They are adapted slightly to fit my implmentation but most of it is the same.

*/

bool Triangle::MTHit(
    Ray& ray,
    float& t, float& u, float& v)
{
    //Vec3 edge1 = vertices[1].pos - vertices[0].pos;
    //Vec3 edge2 = vertices[2].pos - vertices[0].pos;

    Vec3 h = ray.direction.Cross(otherEdge);
    float determinant = edges[0].Dot(h); // Dot product of Ray Direction and h

#if CULLING

    if (determinant < kEPSILON)
        return false;
    Vec3 distFromOrig = (ray.origin - vertices[0].pos);
    u = distFromOrig.Dot(h);
    if (u < 0.0 || u > determinant)
        return false;
    Vec3 q = distFromOrig.Cross(edges[0]);
    v = ray.direction.Dot(q);
    if (v < 0.0 || u + v > determinant)
        return false;
    // At this stage we can compute t to find out where the intersection point is on the line.
    float inv_det = 1.0f / determinant;
    t = otherEdge.Dot(q) * inv_det;
    u *= inv_det;
    v *= inv_det;
    //if (t > (kEPSILON)) // ray intersection
    //    return true;
    //else
    //    return false;
#else

    if (std::fabsf(determinant) < kEPSILON)
        return false;
    float inv_det = 1.0f / determinant;
    Vec3 distFromOrig = (ray.origin - vertices[0].pos);
    u = inv_det * distFromOrig.Dot(h);
    if (u < 0.0 || u > 1.0)
        return false;
    Vec3 q = distFromOrig.Cross(edges[0]);
    v = inv_det * ray.direction.Dot(q);
    if (v < 0.0 || u + v > 1.0)
        return false;
    // At this stage we can compute t to find out where the intersection point is on the line.
    t = inv_det * otherEdge.Dot(q);
#endif
    if (t > (kEPSILON)) // ray intersection
        return true;
    else // This means that there is a line intersection but not a ray intersection.
        return false;
}

bool Triangle::ShadowMTHit(
    Ray& ray,
    float& t, float& u, float& v)
{

    Vec3 h = ray.direction.Cross(otherEdge); // Dot product of Ray Direction and edge2
    float determinant = edges[0].Dot(h); // Dot product of Ray Direction and h

#if CULLING

    if (determinant < kEPSILON)
        return false;
    Vec3 distFromOrig = (ray.origin - vertices[0].pos);
    u = distFromOrig.Dot(h);
    if (u < 0.0 || u > determinant)
        return false;
    Vec3 q = distFromOrig.Cross(edges[0]);
    v = ray.direction.Dot(q);
    if (v < 0.0 || u + v > determinant)
        return false;
    // At this stage we can compute t to find out where the intersection point is on the line.
    float inv_det = 1.0f / determinant;
    t = otherEdge.Dot(q) * inv_det;
    u *= inv_det;
    v *= inv_det;
#else

    if (std::fabsf(determinant) < kEPSILON)
        return false;
    float inv_det = 1.0f / determinant;
    Vec3 distFromOrig = (ray.origin - vertices[0].pos);
    u = inv_det * distFromOrig.Dot(h);
    if (u < 0.0 || u > 1.0)
        return false;
    Vec3 q = distFromOrig.Cross(edges[0]);
    v = inv_det * ray.direction.Dot(q);
    if (v < 0.0 || u + v > 1.0)
        return false;
    // At this stage we can compute t to find out where the intersection point is on the line.
    t = inv_det * otherEdge.Dot(q);
#endif
    if (t > (kEPSILON)) // ray intersection
        return true;
    else // This means that there is a line intersection but not a ray intersection.
        return false;

    //const float EPSILON = 0.0000001;
    //Vec3 edge1, edge2, h, s, q;
    //float a, f;
    //edge1 = vertices[1].pos - vertices[0].pos;
    //edge2 = vertices[2].pos - vertices[0].pos;
    //h = ray.direction.Cross(edge2);
    //a = edge1.Dot(h);
    //if (a > -EPSILON && a < EPSILON)
    //    return false;    // This ray is parallel to this triangle.
    //f = 1.0 / a;
    //s = ray.origin - vertices[0].pos;
    //u = f * s.Dot(h);
    //if (u < 0.0 || u > 1.0)
    //    return false;
    //q = s.Cross(edge1);
    //v = f * ray.direction.Dot(q);
    //if (v < 0.0 || (u + v) > 1.0)
    //    return false;
    //// At this stage we can compute t to find out where the intersection point is on the line.
    //t = f * edge2.Dot(q);
    //if (t > EPSILON) // ray intersection
    //{
    //    Vec3 intersectionPoint = ray.origin + ray.direction * t;

    //    std::string ro = Utilities::Vec3ToString("RayOrigin", ray.origin);
    //    DebugPrinter::Print(ro);
    //    std::string rd = Utilities::Vec3ToString("Ray Direction", ray.direction);
    //    DebugPrinter::Print(rd);
    //    std::string p = Utilities::Vec3ToString("Intersect Point", intersectionPoint);
    //    DebugPrinter::Print(p);
    //    return true;
    //}
    //else // This means that there is a line intersection but not a ray intersection.
    //    return false;
}

bool Triangle::LewisHit(
    Ray& ray,
    float& t, float& u, float& v)
{
    Vec3 edge1 = vertices[1].pos - vertices[0].pos;
    Vec3 edge2 = vertices[2].pos - vertices[0].pos;
    Vec3 norm = edge2.Cross(edge1);

    float d = norm.Dot(vertices[0].pos);
    float nd = norm.Dot(ray.direction);
    if (std::fabsf(nd) < kEPSILON)
        return false;

    t = (d - norm.Dot(ray.origin)) / nd;
    if (t < 0.0)
        return false;
    Vec3 iP = ray.origin + (t * ray.direction);
    norm.Normalise();

    edge2 = vertices[2].pos - vertices[1].pos;
    Vec3 edge3 = vertices[0].pos - vertices[2].pos;

    Vec3 vp1 = iP - vertices[0].pos;
    Vec3 vp2 = iP - vertices[1].pos;
    Vec3 vp3 = iP - vertices[2].pos;

    if (edge1.Cross(vp1).Dot(norm) < 0)
        return false;

    if (edge2.Cross(vp2).Dot(norm) < 0)
        return false;

    if (edge3.Cross(vp3).Dot(norm) < 0)
        return false;

    u = edge2.Cross(vp2).Dot(norm) / edge1.Cross(vp1).Dot(norm);

    v = edge3.Cross(vp3).Dot(norm) / edge1.Cross(vp1).Dot(norm);
    return true;
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
