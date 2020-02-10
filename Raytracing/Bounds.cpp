#include "Bounds.h"

#include <algorithm>

void Bounds::Load(const Mesh& mesh)
{
	max = { FLT_MIN, FLT_MIN, FLT_MIN };
	min = { FLT_MAX, FLT_MAX, FLT_MAX };
	for (size_t i = 0; i < mesh.vertices.size(); ++i)
	{
		max.axis[0] = std::max(max.axis[0], mesh.vertices[i].pos.axis[0]);
		max.axis[1] = std::max(max.axis[1], mesh.vertices[i].pos.axis[1]);
		max.axis[2] = std::max(max.axis[2], mesh.vertices[i].pos.axis[2]);

		min.axis[0] = std::min(min.axis[0], mesh.vertices[i].pos.axis[0]);
		min.axis[1] = std::min(min.axis[1], mesh.vertices[i].pos.axis[1]);
		min.axis[2] = std::min(min.axis[2], mesh.vertices[i].pos.axis[2]);
	}
}

void Bounds::Load(const std::vector<Triangle>& _tris)
{
	max = { -INFINITY, -INFINITY, -INFINITY };
	min = { INFINITY, INFINITY, INFINITY };
	for (size_t i = 0; i < _tris.size(); ++i)
	{
		for (size_t j = 0; j < 3; ++j)
		{
			max.axis[0] = std::max(max.axis[0], _tris[i].vertices[j].pos.axis[0]);
			max.axis[1] = std::max(max.axis[1], _tris[i].vertices[j].pos.axis[1]);
			max.axis[2] = std::max(max.axis[2], _tris[i].vertices[j].pos.axis[2]);										
			min.axis[0] = std::min(min.axis[0], _tris[i].vertices[j].pos.axis[0]);
			min.axis[1] = std::min(min.axis[1], _tris[i].vertices[j].pos.axis[1]);
			min.axis[2] = std::min(min.axis[2], _tris[i].vertices[j].pos.axis[2]);
		}
	}
}

void Bounds::Load(const std::vector<Vec3>& positions)
{

	max = { FLT_MIN, FLT_MIN, FLT_MIN };
	min = { FLT_MAX, FLT_MAX, FLT_MAX };
	for (size_t i = 0; i < positions.size(); ++i)
	{
		max.axis[0] = std::max(max.axis[0], positions[i].axis[0]);
		max.axis[1] = std::max(max.axis[1], positions[i].axis[1]);
		max.axis[2] = std::max(max.axis[2], positions[i].axis[2]);

		min.axis[0] = std::min(min.axis[0], positions[i].axis[0]);
		min.axis[1] = std::min(min.axis[1], positions[i].axis[1]);
		min.axis[2] = std::min(min.axis[2], positions[i].axis[2]);
	}
}

void Bounds::operator=(const Bounds& bb)
{
	max = bb.max;
	min = bb.min;
}
