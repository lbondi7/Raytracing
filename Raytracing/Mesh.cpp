#include "Mesh.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <unordered_map>
#include <random>

void MeshLoading::Load(const std::string& mesh)
{

	meshesID[mesh] = meshCount;
	++meshCount;
	meshes.resize(meshCount);
	//vertices.resize(meshCount);
	//indices.resize(meshCount);

	std::string filePath = "models/" + mesh + ".obj";

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filePath.c_str())) {
		throw std::runtime_error(warn + err);
	}

	std::unordered_map<Vertex, uint32_t> uniqueVertices = {};

	std::random_device rd;
	std::uniform_real_distribution<float> rand(0.0f, 1.0f);

	for (const auto& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {
			Vertex vertex = {};

			vertex.pos = {
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};

			vertex.texCoord = {
				attrib.texcoords[2 * index.texcoord_index + 0],
				1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
			};

			//vertex.normal = {
			//	attrib.normals[3 * index.normal_index + 0],
			//	attrib.normals[3 * index.normal_index + 1],
			//	attrib.normals[3 * index.normal_index + 2]
			//};

			vertex.colour = { 1.0f, 1.0f, 1.0f, 1.0f};

			if (uniqueVertices.count(vertex) == 0) {
				uniqueVertices[vertex] = static_cast<uint32_t>(meshes[meshesID[mesh]].vertices.size());
				meshes[meshesID[mesh]].vertices.push_back(vertex);
			}

			meshes[meshesID[mesh]].indices.push_back(uniqueVertices[vertex]);
		}
	}
}
