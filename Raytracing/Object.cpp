#include "Object.h"
#include "Locator.h"

constexpr float kEpsilon = 1e-8;

Object::Object(Vec3 _center)
{
	center = _center;
}

Object::~Object()
{
}

void Object::Load(const std::string& meshType)
{

	mesh = Locator::GetMeshes()->meshes[Locator::GetMeshes()->meshesID[meshType]];

    //for (size_t i = 0; i < mesh.indices.size(); i+=3)
    //{
    //    Triangle tri(center + mesh.vertices[mesh.indices[i]].pos,
    //        center + mesh.vertices[mesh.indices[i + 1]].pos,
    //        center + mesh.vertices[mesh.indices[i + 2]].pos);

    //    tris.push_back(tri);
    //}

    boundingBox.Load(mesh);
}

void Object::Load(const std::string& meshType, const Vec3& _center)
{
    center = _center;
    mesh = Locator::GetMeshes()->meshes[Locator::GetMeshes()->meshesID[meshType]];

    for (size_t i = 0; i < mesh.indices.size(); i += 3)
    {
        Triangle tri(center, mesh.vertices[mesh.indices[i]],
            mesh.vertices[mesh.indices[i + 1]],
            mesh.vertices[mesh.indices[i + 2]]);

        tris.push_back(tri);
    }

    boundingBox.Load(mesh);
}

Vec3 Object::HitObject(
    const Ray& ray)
{
    float t, u, v, w;
    for (size_t i = 0; i < mesh.indices.size(); i += 3)
    {
        if (HitTriangle(ray,
            center + mesh.vertices[mesh.indices[i]].pos,
            center + mesh.vertices[mesh.indices[i + 1]].pos,
            center + mesh.vertices[mesh.indices[i + 2]].pos, t, u, v))
        {
            w = (1 - u - v);
            return (Vec3(mesh.vertices[mesh.indices[i]].colour) * u) + 
            (Vec3(mesh.vertices[mesh.indices[i + 1]].colour) * v) + 
            (Vec3(mesh.vertices[mesh.indices[i + 2]].colour) * w) ;
        }
    }

    return { -1.0f, -1.0f, -1.0f };
}

bool Object::Hit(const Ray& ray, Vec3& colour)
{
    float t, u, v, w;
    for (size_t i = 0; i < mesh.indices.size(); i += 3)
    {
        if (HitTriangle(ray,
            center + mesh.vertices[mesh.indices[i]].pos,
            center + mesh.vertices[mesh.indices[i + 1]].pos,
            center + mesh.vertices[mesh.indices[i + 2]].pos, t, u, v))
        {
            w = (1 - u - v);
            colour = (Vec3(mesh.vertices[mesh.indices[i]].colour) * u) + 
            (Vec3(mesh.vertices[mesh.indices[i + 1]].colour) * v) + 
            (Vec3(mesh.vertices[mesh.indices[i + 2]].colour) * w) ;
            return true;
        }
    }

    return false;
}

bool Object::Hit(const Ray& ray, HitRecord& rec)
{
    float t, u, v, w;
    for (size_t i = 0; i < mesh.indices.size(); i += 3)
    {
        if (HitTriangle(ray,
            center + mesh.vertices[mesh.indices[i]].pos,
            center + mesh.vertices[mesh.indices[i + 1]].pos,
            center + mesh.vertices[mesh.indices[i + 2]].pos, t, u, v, rec))
        {
            if (rec.t > t)
            {
                w = (1 - u - v);
                rec.colour = (Vec3(mesh.vertices[mesh.indices[i]].colour) * u) +
                    (Vec3(mesh.vertices[mesh.indices[i + 1]].colour) * v) +
                    (Vec3(mesh.vertices[mesh.indices[i + 2]].colour) * w);
                rec.t = t;
                return true;
            }
        }
    }
    return false;
}

void Object::Hit(const Ray& ray, std::vector<HitRecord>& hits)
{
    float t, u, v, w;
    for (size_t i = 0; i < mesh.indices.size(); i += 3)
    {
        if (HitTriangle(ray,
            center + mesh.vertices[mesh.indices[i]].pos,
            center + mesh.vertices[mesh.indices[i + 1]].pos,
            center + mesh.vertices[mesh.indices[i + 2]].pos, t, u, v))
        {
            HitRecord hit;
            hit.t = t;
            w = (1 - u - v);
            hit.colour = (Vec3(mesh.vertices[mesh.indices[i]].colour) * w) +
                (Vec3(mesh.vertices[mesh.indices[i + 1]].colour) * u) +
                (Vec3(mesh.vertices[mesh.indices[i + 2]].colour) * v);
            hits.push_back(hit);
        }
    }
}

bool Object::HitTriangle(
    const Ray& ray,
    const Vec3& v0, const Vec3& v1, const Vec3& v2,
    float& t, float& u, float& v)
{
    // compute plane's normal
    Vec3 v0v1 = v1 - v0;
    Vec3 v0v2 = v2 - v0;
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
    float d = Vec3::Dot(N, v0);

    // compute t (equation 3)
    t = (Vec3::Dot(N, ray.origin) + d) / NdotRayDirection;
    // check if the triangle is in behind the ray
    if (t < 0) return false; // the triangle is behind 

    // compute the intersection point using equation 1
    Vec3 P = ray.origin + t * ray.direction;

    // Step 2: inside-outside test
    Vec3 C; // vector perpendicular to triangle's plane 

    // edge 0
    Vec3 edge0 = v1 - v0;
    Vec3 vp0 = P - v0;
    C = Vec3::Cross(edge0, vp0);
    if (Vec3::Dot(N, C) < 0) return false; // P is on the right side 

    // edge 1
    Vec3 edge1 = v2 - v1;
    Vec3 vp1 = P - v1;
    C = Vec3::Cross(edge1, vp1);
    if ((u = Vec3::Dot(N, C)) < 0)  return false; // P is on the right side 

    // edge 2
    Vec3 edge2 = v0 - v2;
    Vec3 vp2 = P - v2;
    C = Vec3::Cross(edge2, vp2);
    if ((v = Vec3::Dot(N, C)) < 0) return false; // P is on the right side; 

    u /= denom;
    v /= denom;

    return true; // this ray hits the triangle 
}

bool Object::HitTriangle(const Ray& ray, const Vec3& v0, const Vec3& v1, const Vec3& v2, float& t, float& u, float& v, HitRecord& rec)

{
    // compute plane's normal
    Vec3 v0v1 = v1 - v0;
    Vec3 v0v2 = v2 - v0;
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
    float d = Vec3::Dot(N, v0);

    // compute t (equation 3)
    t = (Vec3::Dot(N, ray.origin) + d) / NdotRayDirection;
    // check if the triangle is in behind the ray
    if (t < 0) return false; // the triangle is behind 

    // compute the intersection point using equation 1
    Vec3 P = ray.origin + t * ray.direction;

    // Step 2: inside-outside test
    Vec3 C; // vector perpendicular to triangle's plane 

    // edge 0
    Vec3 edge0 = v1 - v0;
    Vec3 vp0 = P - v0;
    C = Vec3::Cross(edge0, vp0);
    if (Vec3::Dot(N, C) < 0) return false; // P is on the right side 

    // edge 1
    Vec3 edge1 = v2 - v1;
    Vec3 vp1 = P - v1;
    C = Vec3::Cross(edge1, vp1);
    if ((u = Vec3::Dot(N, C)) < 0)  return false; // P is on the right side 

    // edge 2
    Vec3 edge2 = v0 - v2;
    Vec3 vp2 = P - v2;
    C = Vec3::Cross(edge2, vp2);
    if ((v = Vec3::Dot(N, C)) < 0) return false; // P is on the right side; 

    u /= denom;
    v /= denom;

    if (rec.t > t)
    {
        rec.normal = N;
    }

    return true; // this ray hits the triangle 
}
