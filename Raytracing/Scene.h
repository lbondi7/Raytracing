#pragma once

#include "Object.h"
#include "Camera.h"
#include "BVH.h"
#include "Image.h"

#include "SFML/Graphics.hpp"

class Scene
{
public:

	Scene() = default;
	~Scene();

	void Colour(const Ray& ray, Vec3& colour);

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

	float xish = 0.0f;

	std::vector<Object> objects;
	int objCount = 1;

	Image img;

	Camera camera;
	BVH bvh;
	std::vector<Triangle> tris;
};

