#pragma once
#include "Vertex.h"

#include <string>
#include <map>
#include <vector>


struct Mesh
{
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
};

struct MeshLoading
{
	//std::vector<std::vector<Vertex>> vertices;
	//std::vector<std::vector<uint32_t>> indices;

	std::vector<Mesh> meshes;

	std::map<std::string, int> meshesID;

	int meshCount = 0;

	void Load(const std::string& mesh);
};

