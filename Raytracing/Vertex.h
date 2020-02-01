#pragma once

#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

struct Vertex {

	//glm::vec3 pos;
	//glm::vec3 normal;
	//glm::vec2 texCoord;
	//glm::vec4 color;

	Vec3 pos;
	Vec3 normal;
	Vec2 texCoord;
	Vec4 color;

	bool operator==(const Vertex& other) const {
		return pos == other.pos && normal == other.normal && color == other.color && texCoord == other.texCoord;
	}
};

namespace std {
	template<> struct hash<Vertex> {
		size_t operator()(Vertex const& vertex) const {
			return ((hash<glm::vec3>()(glm::vec3(vertex.pos.x, vertex.pos.y, vertex.pos.z)) ^
				(hash<glm::vec3>()(glm::vec3(vertex.normal.x, vertex.normal.y, vertex.normal.z))) ^
				(hash<glm::vec4>()(glm::vec4(vertex.normal.x, vertex.normal.y, vertex.normal.z, vertex.color.w)) << 1)) >> 1) ^
				(hash<glm::vec2>()(glm::vec2(vertex.normal.x, vertex.normal.y)) << 1);
		}
	};
}