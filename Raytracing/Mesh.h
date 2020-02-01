#pragma once
#include "Vertex.h"

#include <string>
#include <map>
#include <vector>

struct Mesh
{
	std::vector<std::vector<Vertex>> vertices;
	std::vector<std::vector<uint32_t>> indices;

	std::map<std::string, int> meshes;

	int meshCount = 0;

	void Load(const std::string& mesh);
};

