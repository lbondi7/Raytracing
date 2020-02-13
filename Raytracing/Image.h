#pragma once

#include "SFML/Graphics.hpp"
#include "Vec3.h"

#include <string>

struct Image
{

	void Load(const std::string& filepath);
	Vec3 GetColour(float u, float v);


	sf::Image image;

};

