#include "Scene.h"
#include "Locator.h"
#include "ThreadManager.h"
#include "Constants.h"

#include <imgui.h>

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
    Locator::GetMeshes()->Load("square");
    Locator::GetMeshes()->Load("triangle");
    Locator::GetMeshes()->Load("iso");


#if THREADED_BUILD
    Locator::InitTM(new ThreadManager(7));
#elif THREADED_TRAVERSE
    Locator::InitTM(new ThreadManager(7));
#endif

    sf::Event event;

    Output output;

    output.Update(1);
    scene.Init(&window);
    while (!scene.Exit())
    {
        output.Update(2);
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                scene.Exit(true);
                output.Outputy();
            }

        }
        scene.Update();

        scene.Render();
        window.display();
    }

    return 0;
}