#include "Vec3.h"
#include "Ray.h"
#include "Utilities.h"

#include "SFML/Graphics.hpp"

#include <random>
#include <algorithm>

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

Vec3 Colour(const Ray& ray){

    if (HitSphere(Vec3(1, 0, 0.5f), 0.3f, ray))
        return Vec3(1, 0, 0);

    if (HitCube(Vec3(5.f, 0, 4), Vec3(2, 2, 2), ray))
        return Vec3(1, 1, 0);

    //if (HitSphere(Vec3(0, -20, 10), 20.f, ray))
    //    return Vec3(0, 1, 0);

    Vec3 unitDirection = UnitVector(ray.direction);
    float t = 0.5f * (unitDirection.y + 1.0f);
    return (1.0f - t) * Vec3(1.0f, 1.0f, 1.0f) + t * Vec3(0.5f, 0.7f, 1.0f);
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(720, 405), "SFML works!");
    window.setFramerateLimit(60);
    sf::Sprite sprite;

    int width , height;

    width = 720;
    height = 405;

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