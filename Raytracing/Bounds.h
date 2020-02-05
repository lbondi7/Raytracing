#pragma once

#include "Vec3.h"
#include "Mesh.h"

struct Bounds
{

	void Load(const Mesh& mesh);

private:
	Vec3 max = { 0.0f, 0.0f, 0.0f };
	Vec3 min = { 0.0f, 0.0f, 0.0f };
};

