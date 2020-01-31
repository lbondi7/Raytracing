#pragma once

#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"

struct Vertex {

	Vec3 pos;
	Vec3 normal;
	Vec2 texCoord;
	Vec4 color;



	bool operator==(const Vertex& other) const {
		return pos == other.pos && normal == other.normal && color == other.color && texCoord == other.texCoord;
	}
};