#include "Scene.h"
#include "Locator.h"
#include "Constants.h"

//bool HitSphere(const Vec3& center, float radius, const Ray& ray){
//    Vec3 oc = ray.origin - center;
//    float a = Vec3::Dot(ray.direction, ray.direction);
//    float b = 2.0f * Vec3::Dot(oc, ray.direction);
//    float c = Vec3::Dot(oc, oc) - radius * radius;
//    float discriminant = b * b - 4 * a * c;
//    return discriminant > 0.0f;
//}
//bool HitCube(const Vec3& center, const Vec3& dims, const Ray& ray) {
//    Vec3 upper(center.x + (dims.x / 2), center.y + (dims.y / 2), center.z + (dims.z / 2));
//    Vec3 lower(center.x - (dims.x / 2), center.y - (dims.y / 2), center.z - (dims.z / 2));
//
//    Vec3 max;
//    Vec3 min;
//    max.x = (upper.x - ray.origin.x) / ray.direction.x;
//    max.y = (upper.y - ray.origin.y) / ray.direction.y;
//    max.z = (upper.z - ray.origin.z) / ray.direction.z;
//
//    min.x = (lower.x + ray.origin.x) / ray.direction.x;
//    min.y = (lower.y + ray.origin.y) / ray.direction.y;
//    min.z = (lower.z + ray.origin.z) / ray.direction.z;
//
//
//    float tmin = (min.x - ray.origin.x) / ray.direction.x;
//    float tmax = (max.x - ray.origin.x) / ray.direction.x;
//
//    if (tmin > tmax) std::swap(tmin, tmax);
//
//    float tymin = (min.y - ray.origin.y) / ray.direction.y;
//    float tymax = (max.y - ray.origin.y) / ray.direction.y;
//
//    if (tymin > tymax) std::swap(tymin, tymax);
//
//    if ((tmin > tymax) || (tymin > tmax))
//        return false;
//
//    if (tymin > tmin)
//        tmin = tymin;
//
//    if (tymax < tmax)
//        tmax = tymax;
//
//    float tzmin = (min.z - ray.origin.z) / ray.direction.z;
//    float tzmax = (max.z - ray.origin.z) / ray.direction.z;
//
//    if (tzmin > tzmax) std::swap(tzmin, tzmax);
//
//    if ((tmin > tzmax) || (tzmin > tmax))
//        return false;
//
//    if (tzmin > tmin)
//        tmin = tzmin;
//
//    if (tzmax < tmax)
//        tmax = tzmax;
//
//    return true;
//
//
//    //float a = Vec3::Dot(ray.direction, ray.direction);
//    //float b = 2.0f * Vec3::Dot(oc, ray.direction);
//    //float c = Vec3::Dot(oc, oc) - radius * radius;
//    //float discriminant = b * b - 4 * a * c;
//    //return discriminant > 0.0f;
//}
//bool HitTriangle(
//    const Vec3& orig, const Vec3& dir,
//    const Vec3& v0, const Vec3& v1, const Vec3& v2,
//    float& t, float& u, float& v)
//{
//    // compute plane's normal
//    Vec3 v0v1 = v1 - v0;
//    Vec3 v0v2 = v2 - v0;
//    // no need to normalize
//    Vec3 N = Vec3::Cross(v0v1, v0v2); // N 
//    float denom = Vec3::Dot(N, N);
//    
//    float area2 = N.Length();
//
//    // Step 1: finding P
//
//    // check if ray and plane are parallel ?
//    float NdotRayDirection = Vec3::Dot(N, dir);
//    if (fabs(NdotRayDirection) < kEpsilon) // almost 0 
//        return false; // they are parallel so they don't intersect ! 
//
//    // compute d parameter using equation 2
//    float d = Vec3::Dot(N, v0);
//
//    // compute t (equation 3)
//    t = (Vec3::Dot(N, orig) + d) / NdotRayDirection;
//    // check if the triangle is in behind the ray
//    if (t < 0) return false; // the triangle is behind 
//
//    // compute the intersection point using equation 1
//    Vec3 P = orig + t * dir;
//
//    // Step 2: inside-outside test
//    Vec3 C; // vector perpendicular to triangle's plane 
//
//    // edge 0
//    Vec3 edge0 = v1 - v0;
//    Vec3 vp0 = P - v0;
//    C = Vec3::Cross(edge0, vp0);
//    if (Vec3::Dot(N, C) < 0) return false; // P is on the right side 
//
//    // edge 1
//    Vec3 edge1 = v2 - v1;
//    Vec3 vp1 = P - v1;
//    C = Vec3::Cross(edge1, vp1);
//    if ((u = Vec3::Dot(N, C)) < 0)  return false; // P is on the right side 
//
//    // edge 2
//    Vec3 edge2 = v0 - v2;
//    Vec3 vp2 = P - v2;
//    C = Vec3::Cross(edge2, vp2);
//    if ((v = Vec3::Dot(N, C)) < 0) return false; // P is on the right side; 
//
//    u /= denom;
//    v /= denom;
//
//    return true; // this ray hits the triangle 
//}
//
//Vec3 Colour(const Ray& ray){
//
//    if (HitSphere(Vec3(1, 0, 4.0f), 0.3f, ray))
//        return Vec3(1, 0, 0);
//
//    float t, u, v, w;
//
//    float dist = Vec3::Distance(ray.origin, center);
//    for (size_t i = 0; i < indices.size(); i += 3)
//    {
//        auto dist1 = Vec3::Distance(ray.origin, center + vertices[indices[i]].pos);
//        auto dist2 = Vec3::Distance(ray.origin, center + vertices[indices[i + 1]].pos);
//        auto dist3 = Vec3::Distance(ray.origin, center + vertices[indices[i + 2]].pos);
//        if ((dist1 > dist) &&
//            (dist2 > dist) &&
//            (dist3 > dist))
//            continue;
//
//        if (HitTriangle(ray.origin, ray.direction,
//            center + vertices[indices[i]].pos,
//            center + vertices[indices[i + 1]].pos,
//            center + vertices[indices[i + 2]].pos, t, u, v))
//        {
//            w = (1 - u - v);
//            Vec3 col1 = (Vec3(vertices[indices[i]].colour) * u);
//            Vec3 col2 = (Vec3(vertices[indices[i + 1]].colour) * v);
//            Vec3 col3 = (Vec3(vertices[indices[i + 2]].colour) * w);
//            Vec3 cool = col1 + col2 + col3;
//            return cool;
//        }
//    }
//
//    Vec3 unitDirection = UnitVector(ray.direction);
//    t = 0.5f * (unitDirection.y + 1.0f);
//    return (1.0f - t) * Vec3(1.0f, 1.0f, 1.0f) + t * Vec3(0.5f, 0.7f, 1.0f);
//}

int main()
{
    Scene scene;

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SFML works!");
    window.setFramerateLimit(60);

    Locator::InitMeshes(new MeshLoading());
    Locator::GetMeshes()->Load("iso");

    sf::Event event;

    scene.Init(&window);

    while (!scene.Exit())
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        scene.Update();
        scene.Render();
    }


    return 0;
}