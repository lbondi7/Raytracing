#include "Scene.h"
#include "Vec3.h"
#include "Locator.h"
#include "Constants.h"
#include "HitRecord.h"
#include "Utilities.h"
#include "ThreadManager.h"
#include "DebugPrinter.h"

#include <random>
#include <cmath>
#include <algorithm>

#define M_SAMP 0;

std::random_device rd;

#if M_SAMP
std::uniform_real_distribution<float> sampleRange(0.0f, 1.0f);
const int sampleNum = 20;

float sampleShit[20]{0.0f};
#endif // MULTI_SAMPLING



Scene::~Scene()
{
}

#if M_SAMP
void SetUpSampleShit()
{
    for (size_t i = 0; i < sampleNum; ++i)
    {
        sampleShit[i] = sampleRange(rd);
    }
}
#endif // MULTI_SAMPLING

#if THREADED_TRAVERSE
void ColourBVH(Image* img, BVH* bvh, Ray* ray, std::vector<Light>*lights, Vec3* colour)
{
    std::vector<HitRecord> hits;

    //bvh->Search(ray, hits, img);
    bvh->Search(*ray, hits, *lights, *img);

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
        *colour = tempHit.colour;
        return;
    }

    float t;
    Vec3 unitDirection = Normalise(ray->direction);
    t = 0.5f * (unitDirection.axis[1] + 1.0f);
    *colour = (1.0f - t) * Vec3(1.0f, 1.0f, 1.0f) + t * Vec3(0.5f, 0.7f, 1.0f);
}

void ThreadFunction(int start, int end, Camera* cam, Image* img, BVH* bvh, std::vector<Light>* lights, sf::Image* image)
{
    int ir, ig, ib;
    for (int x = 0; x < WIDTH; ++x)
    {
        for (int y = end - 1; y >= start; --y)
        {

#if M_SAMP
            Vec3 colour = Vec3(0.0f, 0.0f, 0.0f);
            for (size_t k = 0; k < sampleNum; k++)
            {
                float u = static_cast<float>(x + sampleShit[k]) / static_cast<float>(WIDTH);
                float v = static_cast<float>(y + sampleShit[k]) / static_cast<float>(HEIGHT);

                Ray ray(cam->position, (cam->lower_left_corner + u * cam->horizontal + v * cam->vertical) - cam->position);
                Vec3 col;
                ColourBVH(img, bvh, &ray, lights, &col);

                colour += col;
            }
            colour /= float(sampleNum);
            ir = static_cast<int>(255.99f * colour.axis[0]);
            ig = static_cast<int>(255.99f * colour.axis[1]);
            ib = static_cast<int>(255.99f * colour.axis[2]);
            image->setPixel(x, y, sf::Color(ir, ig, ib, 255));
#else

        float u = static_cast<float>(x) / static_cast<float>(WIDTH);
        float v = static_cast<float>(y) / static_cast<float>(HEIGHT);

        Ray ray(cam->position, (cam->lower_left_corner + u * cam->horizontal + v * cam->vertical) - cam->position);
        Vec3 col;

        ColourBVH(img, bvh, &ray, lights, &col);

        ir = static_cast<int>(255.99f * col.axis[0]);
        ig = static_cast<int>(255.99f * col.axis[1]);
        ib = static_cast<int>(255.99f * col.axis[2]);
        image->setPixel(x, y, sf::Color(ir, ig, ib, 255));

#endif // MULTI_SAMPLING

        }
    }
}

#else
void Scene::Colour(const Ray& ray, Vec3& colour)
{
    std::vector<HitRecord> hits;
    Ray _ray(ray.origin, ray.direction);
    bvh.Search(_ray, hits, lights, img);

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
#endif
void Scene::Init(sf::RenderWindow* _window)
{
	window = _window;

    image.create(WIDTH, HEIGHT, sf::Color::White);
    texture.loadFromImage(image);
    sprite.setTexture(texture);

    //camera.SetPos({ 15, 0, -5 });
    //camera.SetLookAt(camera.GetPos() + Vec3(0, 0, 1));
    //camera.SetViewMatrix();

    Vec3 lookat = Vec3(0.0f, 0.0f, 0.0f);
    Vec3 lookfrom = Vec3(-2.0f, 0.0f, 6.0f);
    Vec3 up = Vec3(0.0f, 1.0f, 0.0f);
    camera.Setup(lookfrom, lookat, up, 60.0f, ASPECT_RATIO, 1.0f);

    std::random_device rd;
    std::uniform_real_distribution<float> rand(-10.0f, 10.0f);

    lights.resize(1);

    for (auto& l : lights)
    {
        l.colour = Vec3(1.0f * 255.0f, 0.6f * 255.0f, 0.0f * 255.0f);
        l.position = Vec3(-10.0f, 3.0f, -20.0f);
        l.size = Vec3(0, 3, 3);
        l.intensity = 2.0f;
    }

    objects.resize(objCount);

    for (int i = 0; i < objCount; ++i)
    {
        if (i == 0)
            objects[i].Load("cube", Vec3(4.0f, 0.0f, -1.8f), i);
        else if(i == 1)
            objects[i].Load("dog2", Vec3(0.0f, 0.0f, -5.0f), i);
    }

    light.Load("cube", Vec3(-10.0f, 3.0f, -20.0f), 2);
    light.isLight = true;
    objects.emplace_back(light);

    for (auto& o : objects)
    {
        if (o.isLight)
        {
            for (auto&t : o.tris)
            {
                t.isLight = true;
            }
        }
        tris.insert(tris.end(), o.tris.begin(), o.tris.end());
    }

    img.Load("textures/perf.jpg");

    //Output output;

    bvh.Create(&tris);
    bvh.Build();

#if M_SAMP
    SetUpSampleShit();
#endif // MULTI_SAMPLING

}

void Scene::Update()
{

}

void Scene::Render()
{
	window->clear();

    RenderImage();
	window->draw(sprite);
    //window->display();
}

void Scene::RenderImage()
{
#if THREADED_TRAVERSE
    const int threadCount = 4;

    //std::thread threads[threadCount];

    int amountPerThread = (HEIGHT / threadCount) + 1;
    int start = 0;
    int end = start + amountPerThread;
    if (end > HEIGHT)
        end = HEIGHT;

    for (int i = 0; i < threadCount; i++)
    {
        Locator::GetTM()->SetTask() = std::bind(ThreadFunction, start, end, &camera, &img, &bvh, &lights, &image);

        start += amountPerThread;
        end = start + amountPerThread;
        if (end > HEIGHT)
            end = HEIGHT;

    }
    Locator::GetTM()->WaitForAllThreads();

    //for (int i = 0; i < threadCount; i++)
    //{
    //   threads[i].join();
    //}
#else

    Vec3 lowerLeftCorner(-2.0f, 1.0f, 1.0f);
    Vec3 horizontal(4.0f, 0.0f, 0.0f);
    Vec3 vertical(0.0f, -2.0f, 0.0f);
    Vec3 origin(0.0f, 0.0f, 0.0f);
    int ir, ig, ib;

    for (int j = 0; j < HEIGHT; ++j)
    {
        for (int i = 0; i < WIDTH; ++i)
        {
            float u = (float(i) / float(WIDTH));
            float v = float(j) / float(HEIGHT);
            Ray ray(camera.position, (camera.lower_left_corner + u * camera.horizontal + v * camera.vertical) - camera.position);
            Vec3 col;
            Colour(ray, col);

            ir = int(255.99f * col.axis[0]);
            ig = int(255.99f * col.axis[1]);
            ib = int(255.99f * col.axis[2]);
            image.setPixel(i, j, sf::Color(ir, ig, ib, 255));
    }
}
#endif
    image.flipVertically();
    texture.update(image);
}
