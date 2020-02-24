#include "Scene.h"
#include "Vec3.h"
#include "Locator.h"
#include "Constants.h"
#include "HitRecord.h"
#include "Utilities.h"

#include <random>

Scene::~Scene()
{
}

void Scene::Colour(const Ray& ray, Vec3& colour)
{
    std::vector<HitRecord> hits;

    bvh.Search(ray, hits, img);

    if (!hits.empty())
    {
        HitRecord tempHit;
        tempHit.t = FLT_MAX;
        for (const auto& hit : hits)
        {
            if (tempHit.t > hit.t)
            {
                tempHit.t = hit.t;
                tempHit.colour = hit.colour;
            }
        }
        colour = tempHit.colour;
        return;
    }

    float t;
    Vec3 unitDirection = Normalise(ray.direction);
    t = 0.5f * (unitDirection.axis[1] + 1.0f);
    colour = (1.0f - t) * Vec3(1.0f, 1.0f, 1.0f) + t * Vec3(0.5f, 0.7f, 1.0f);
}

void Scene::Colour(BVH& bvh, const Ray& ray, Vec3& colour)
{
    std::vector<HitRecord> hits;

    bvh.Search(ray, hits, img);

    if (!hits.empty())
    {
        HitRecord tempHit;
        tempHit.t = FLT_MAX;
        for (const auto& hit : hits)
        {
            if (tempHit.t > hit.t)
            {
                tempHit.t = hit.t;
                tempHit.colour = hit.colour;
            }
        }
        colour = tempHit.colour;
        return;
    }

    float t;
    Vec3 unitDirection = Normalise(ray.direction);
    t = 0.5f * (unitDirection.axis[1] + 1.0f);
    colour = (1.0f - t) * Vec3(1.0f, 1.0f, 1.0f) + t * Vec3(0.5f, 0.7f, 1.0f);
}

void Scene::Init(sf::RenderWindow* _window)
{
	window = _window;

    image.create(WIDTH, HEIGHT, sf::Color::White);
    texture.loadFromImage(image);
    sprite.setTexture(texture);

    camera.SetPos({ 0, 0, 0 });
    camera.SetLookAt(camera.GetPos() + Vec3(0, 0, 1));
    camera.SetViewMatrix();

    std::random_device rd;
    std::uniform_real_distribution<float> rand(-10.0f, 10.0f);

    objects.resize(objCount);

    for (size_t i = 0; i < objCount; ++i)
    {
       //objects[i].Load("cube2", Vec3(i, i, i + 4));
       objects[i].Load("cube", Vec3(i * 3, 0, 4));
       //objects[i].Load("dog2", Vec3(i, -3, i + 4));
    }

    light.Load("cube", Vec3());
    light.isLight = true;
    light.SetCenter(Vec3(0, 0, 0));
    //objects.emplace_back(light);

    for (size_t i = 0; i < objects.size(); ++i)
    {
        if (objects[i].isLight)
        {
            for (size_t j = 0; j < objects[i].tris.size(); ++j)
            {
                objects[i].tris[j].isLight = true;
            }
        }
        tris.insert(tris.end(), objects[i].tris.begin(), objects[i].tris.end());
    }

    img.Load("textures/perf.jpg");

    bvh.Create(&tris);
    bvh.Build();

    Vec3 lowerLeftCorner(2.0f, 1.0f, 1.0f);
    Vec3 horizontal(-4.0f, 0.0f, 0.0f);
    Vec3 vertical(0.0f, -2.0f, 0.0f);
    Vec3 origin(0.0f, 0.0f, 0.0f);
    int ir, ig, ib;
    for (int j = 0; j < HEIGHT; ++j)
    {
        for (int i = 0; i < WIDTH; ++i)
        {
            float u = float(i) / float(WIDTH);
            float v = float(j) / float(HEIGHT);
            Ray ray(origin, lowerLeftCorner + u * horizontal + v * vertical);
            Vec3 col;
            Colour(ray, col);

            ir = int(255.99f * col.axis[0]);
            ig = int(255.99f * col.axis[1]);
            ib = int(255.99f * col.axis[2]);
            image.setPixel(i, j, sf::Color(ir, ig, ib, 255));
        }
    }
    texture.update(image);
}

void Scene::Update()
{
    //BVH bvhs;
    //bvhs.Create(&tris);
    //bvhs.Build();

    Vec3 lowerLeftCorner(-2.0f, 1.0f, 1.0f);
    Vec3 horizontal(4.0f, 0.0f, 0.0f);
    Vec3 vertical(0.0f, -2.0f, 0.0f);
    Vec3 origin(0.0f, 0.0f, 0.0f);
    int ir = 0;
    int ig = 0;
    int ib = 0;
    for (int j = 0; j < HEIGHT; ++j)
    {
        for (int i = 0; i < WIDTH; ++i)
        {
            float u = float(i) / float(WIDTH);
            float v = float(j) / float(HEIGHT);
            Ray ray(origin, lowerLeftCorner + u * horizontal + v * vertical);
            Vec3 col;
            Colour(ray, col);

            ir = int(255.99f * col.axis[0]);
            ig = int(255.99f * col.axis[1]);
            ib = int(255.99f * col.axis[2]);
            image.setPixel(i, j, sf::Color(ir, ig, ib, 255));
        }
    }
    texture.update(image);
}
void Scene::Render()
{
	window->clear();


	window->draw(sprite);
	window->display();
}
