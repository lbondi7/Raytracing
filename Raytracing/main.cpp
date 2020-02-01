#include "Vec3.h"
#include "Ray.h"
#include "Utilities.h"
#include "Mesh.h"

#include "SFML/Graphics.hpp"

#include <random>
#include <algorithm>

constexpr float kEpsilon = 1e-8;

Vec3 center(0, 0, 4);

Mesh obj;

std::vector<Vertex> vertices;
std::vector<uint32_t> indices;

bool HitSphere(const Vec3& center, float radius, const Ray& ray){
    Vec3 oc = ray.origin - center;
    float a = Vec3::Dot(ray.direction, ray.direction);
    float b = 2.0f * Vec3::Dot(oc, ray.direction);
    float c = Vec3::Dot(oc, oc) - radius * radius;
    float discriminant = b * b - 4 * a * c;
    return discriminant > 0.0f;
}

bool HitCube(const Vec3& center, const Vec3& dims, const Ray& ray) {
    Vec3 upper(center.x + (dims.x / 2), center.y + (dims.y / 2), center.z + (dims.z / 2));
    Vec3 lower(center.x - (dims.x / 2), center.y - (dims.y / 2), center.z - (dims.z / 2));

    Vec3 max;
    Vec3 min;
    max.x = (upper.x - ray.origin.x) / ray.direction.x;
    max.y = (upper.y - ray.origin.y) / ray.direction.y;
    max.z = (upper.z - ray.origin.z) / ray.direction.z;

    min.x = (lower.x + ray.origin.x) / ray.direction.x;
    min.y = (lower.y + ray.origin.y) / ray.direction.y;
    min.z = (lower.z + ray.origin.z) / ray.direction.z;


    float tmin = (min.x - ray.origin.x) / ray.direction.x;
    float tmax = (max.x - ray.origin.x) / ray.direction.x;

    if (tmin > tmax) std::swap(tmin, tmax);

    float tymin = (min.y - ray.origin.y) / ray.direction.y;
    float tymax = (max.y - ray.origin.y) / ray.direction.y;

    if (tymin > tymax) std::swap(tymin, tymax);

    if ((tmin > tymax) || (tymin > tmax))
        return false;

    if (tymin > tmin)
        tmin = tymin;

    if (tymax < tmax)
        tmax = tymax;

    float tzmin = (min.z - ray.origin.z) / ray.direction.z;
    float tzmax = (max.z - ray.origin.z) / ray.direction.z;

    if (tzmin > tzmax) std::swap(tzmin, tzmax);

    if ((tmin > tzmax) || (tzmin > tmax))
        return false;

    if (tzmin > tmin)
        tmin = tzmin;

    if (tzmax < tmax)
        tmax = tzmax;

    return true;


    //float a = Vec3::Dot(ray.direction, ray.direction);
    //float b = 2.0f * Vec3::Dot(oc, ray.direction);
    //float c = Vec3::Dot(oc, oc) - radius * radius;
    //float discriminant = b * b - 4 * a * c;
    //return discriminant > 0.0f;
}

bool HitTriangle(
    const Vec3& orig, const Vec3& dir,
    const Vec3& v0, const Vec3& v1, const Vec3& v2,
    float& t)
{
    // compute plane's normal
    Vec3 v0v1 = v1 - v0;
    Vec3 v0v2 = v2 - v0;
    // no need to normalize
    Vec3 N = Vec3::Cross(v0v1, v0v2); // N 
    float area2 = N.Length();

    // Step 1: finding P

    // check if ray and plane are parallel ?
    float NdotRayDirection = Vec3::Dot(N, dir);
    if (fabs(NdotRayDirection) < kEpsilon) // almost 0 
        return false; // they are parallel so they don't intersect ! 

    // compute d parameter using equation 2
    float d = Vec3::Dot(N, v0);

    // compute t (equation 3)
    t = (Vec3::Dot(N, orig) + d) / NdotRayDirection;
    // check if the triangle is in behind the ray
    if (t < 0) return false; // the triangle is behind 

    // compute the intersection point using equation 1
    Vec3 P = orig + t * dir;

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
    if (Vec3::Dot(N, C) < 0)  return false; // P is on the right side 

    // edge 2
    Vec3 edge2 = v0 - v2;
    Vec3 vp2 = P - v2;
    C = Vec3::Cross(edge2, vp2);
    if (Vec3::Dot(N, C) < 0) return false; // P is on the right side; 

    return true; // this ray hits the triangle 
}

Vec3 Colour(const Ray& ray){

    if (HitSphere(Vec3(1, 0, 0.5f), 0.3f, ray))
        return Vec3(1, 0, 0);

    float t;

    for (size_t i = 0; i < indices.size(); i += 3)
    {
        if (HitTriangle(ray.origin, ray.direction, center + vertices[indices[i]].pos,
            center +  vertices[indices[i + 1]].pos,
            center +  vertices[indices[i + 2]].pos, t))
            return Vec3(1, 1, 0);
    }

    Vec3 unitDirection = UnitVector(ray.direction);
    t = 0.5f * (unitDirection.y + 1.0f);
    return (1.0f - t) * Vec3(1.0f, 1.0f, 1.0f) + t * Vec3(0.5f, 0.7f, 1.0f);
}

int main()
{
    int width, height;

    //width = 1280;
    //height = 720;

    width = 720;
    height = 405;

    sf::RenderWindow window(sf::VideoMode(width, height), "SFML works!");
    window.setFramerateLimit(60);
    sf::Sprite sprite;

    obj.Load("cylinder");
    vertices = obj.vertices[obj.meshes["cylinder"]];
    indices = obj.indices[obj.meshes["cylinder"]];

    sf::Image image;
    image.create(width, height, sf::Color::White);
    sf::Texture texture;
    texture.loadFromImage(image);
    sprite.setTexture(texture);
    sprite.setPosition(sf::Vector2f(width, height));
    sprite.rotate(180);
    //rect.setTextureRect(sf::IntRect({ 0, 0 }, { (int)rect.getGlobalBounds().width, (int)rect.getGlobalBounds().height }));
   // sprite.setTexture(texture);


    Vec3 lowerLeftCorner(-2.0f, -1.0f, 1.0f);
    Vec3 horizontal(4.0f, 0.0f, 0.0f);
    Vec3 vertical(0.0f, 2.0f, 0.0f);
    Vec3 origin(0.0f, 0.0f, 0.0f);
    for (int j = height - 1; j >= 0; j--)
    {
        for (int i = 0; i < width; i++)
        {
            float u = float(i) / float(width);
            float v = float(j) / float(height);
            Ray ray(origin, lowerLeftCorner + u * horizontal + v * vertical);
            Vec3 col = Colour(ray);
            int ir = int(254.99f * col.x);
            int ig = int(254.99f * col.y);
            int ib = int(254.99f * col.z);
            image.setPixel(i, j, sf::Color(ir, ig, ib, 255));
        }
    }
    texture.update(image);


    std::random_device rd;
    std::uniform_int_distribution<unsigned int> colour(0, 255);
    std::uniform_int_distribution<unsigned int> pos(0, height);

    sf::Event event;
    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

       // image.setPixel(pos(rd), pos(rd), sf::Color(colour(rd), colour(rd), colour(rd), 255));
      //  texture.update(image);

        window.clear();
        window.draw(sprite);
        window.display();
    }

    return 0;
}