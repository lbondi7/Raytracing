#pragma once
#include "Vertex.h"

#include <string>


struct Mesh
{

	std::vector<std::vector<Vertex>> vertices;
	std::vector<std::vector<uint32_t>> indices;

	void Load(const std::string& mesh);
};

