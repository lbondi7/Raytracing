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

    bvh.Search(ray, hits);

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


    //float valMax = Utilities::fPercentageFromMax(-2.5f, 2.5f, -2.5f);
    //float valMin = Utilities::fPercentageFromMax(-2.5f, 2.5f, 0.63f);
    //Utilities::Print(valMax);
    //Utilities::Print(valMin);

    std::random_device rd;
    std::uniform_real_distribution<float> rand(-10.0f, 10.0f);

    objects.resize(objCount);

    for (size_t i = 0; i < objCount; ++i)
    {
        //if(i % 1 && i != 0)
        //    objects[i].Load("iso", Vec3(rand(rd), rand(rd), i * 5));
        //else
            objects[i].Load("iso", Vec3(rand(rd), rand(rd), i + 5));
    }

    for (size_t i = 0; i < objects.size(); ++i)
    {
        for (size_t j = 0; j < objects[i].tris.size(); ++j)
        {
            tris.push_back(objects[i].tris[j]);
        }
    }

    bvh.Create(&tris);
    bvh.Build();

    Vec3 lowerLeftCorner(-2.0f, -1.0f, 1.0f);
    Vec3 horizontal(4.0f, 0.0f, 0.0f);
    Vec3 vertical(0.0f, 2.0f, 0.0f);
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

    //float t;
//Vec3 unitDirection = Normalise(ray.direction);
//t = 0.5f * (unitDirection.axis[1] + 1.0f);
//col =(1.0f - t) * Vec3(1.0f, 1.0f, 1.0f) + t * Vec3(0.5f, 0.7f, 1.0f);

}

void Scene::Update()
{

}

void Scene::Render()
{
	window->clear();


	window->draw(sprite);
	window->display();
}
