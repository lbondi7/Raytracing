#pragma once

#include "Object.h"
#include "Camera.h"
#include "BVH.h"
#include "Image.h"
#include "Light.h"
#include "Output.h"

#include "SFML/Graphics.hpp"


class Scene
{
public:

	Scene() = default;
	~Scene();

	void Colour(const Ray& ray, Vec3& colour);

	//void Colour(BVH& bvh, const Ray& ray, Vec3& colour);

	void Init(sf::RenderWindow* _window);

	void Update();

	void Render();

	void RenderImage();

	bool Exit() { return exit; }

	void Exit(bool _exit) { exit = _exit; }

private:

	bool exit = false;

	sf::RenderWindow* window = nullptr;
	sf::Sprite sprite;
	sf::Image image;
	sf::Texture texture;

	std::vector<Object> objects;
	int objCount = 2;

	Image img;

	Camera camera;
	BVH bvh;
	std::vector<Triangle> tris;
	Object light;
	std::vector<Light> lights;
};

