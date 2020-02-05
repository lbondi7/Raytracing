#include "Scene.h"
#include "Vec3.h"
#include "Locator.h"
#include "Constants.h"

Scene::~Scene()
{
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

    obj.Load("iso");
    obj.center = { 0, 0, 3 };

    Vec3 lowerLeftCorner(-2.0f, -1.0f, 1.0f);
    Vec3 horizontal(4.0f, 0.0f, 0.0f);
    Vec3 vertical(0.0f, 2.0f, 0.0f);
    Vec3 origin(0.0f, 0.0f, 0.0f);
    int ir, ig, ib;
    for (int j = HEIGHT - 1; j >= 0; j--)
    {
        for (int i = 0; i < WIDTH; i++)
        {
            float u = float(i) / float(WIDTH);
            float v = float(j) / float(HEIGHT);
            Ray ray(origin, lowerLeftCorner + u * horizontal + v * vertical);
            Vec3 col = obj.HitObject(ray);
            ir = int(255.99f * col.x);
            ig = int(255.99f * col.y);
            ib = int(255.99f * col.z);
            image.setPixel(i, j, sf::Color(ir, ig, ib, 255));
        }
    }
    texture.update(image);
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
