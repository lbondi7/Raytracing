#include "Scene.h"
#include "Locator.h"
#include "ThreadManager.h"
#include "Constants.h"

int main()
{
    Scene scene;

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SFML works!");
    window.setFramerateLimit(60);

    Locator::InitMeshes(new MeshLoading());
    Locator::GetMeshes()->Load("cylinder");
    Locator::GetMeshes()->Load("cube");
    Locator::GetMeshes()->Load("cube2");
    Locator::GetMeshes()->Load("dog");
    Locator::GetMeshes()->Load("dog2");
    Locator::GetMeshes()->Load("iso");
    Locator::GetMeshes()->Load("triangle");

    Locator::InitTM(new ThreadManager(6));

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