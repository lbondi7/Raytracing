#include "Bounds.h"

#include <algorithm>

void Bounds::Load(const Mesh& mesh)
{
	for (size_t i = 0; i < mesh.vertices.size(); i++)
	{
		max.x = std::max(max.x, mesh.vertices[i].pos.x);
		max.y = std::max(max.y, mesh.vertices[i].pos.y);
		max.z = std::max(max.z, mesh.vertices[i].pos.z);

		min.x = std::min(min.x, mesh.vertices[i].pos.x);
		min.y = std::min(min.y, mesh.vertices[i].pos.y);
		min.z = std::min(min.z, mesh.vertices[i].pos.z);
	}
}
