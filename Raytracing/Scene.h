#pragma once

#include "Object.h"
#include "Camera.h"

#include "SFML/Graphics.hpp"

class Scene
{
public:

	Scene() = default;
	~Scene();

	void Init(sf::RenderWindow* _window);

	void Update();

	void Render();

	bool Exit() { return exit; }

private:

	bool exit = false;

	sf::RenderWindow* window = nullptr;
	sf::Sprite sprite;
	sf::Image image;
	sf::Texture texture;

	Object obj;

	Camera camera;

};

