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
	Vec4 colour;

	bool operator==(const Vertex& other) const {
		return pos == other.pos && colour == other.colour && texCoord == other.texCoord;
	}
};

//namespace std {
//	template<> struct hash<Vertex> {
//		size_t operator()(Vertex const& vertex) const {
//			return ((hash<glm::vec3>()(glm::vec3(vertex.pos.axis[0], vertex.pos.axis[1], vertex.pos.axis[2])) ^
//				(hash<glm::vec3>()(glm::vec3(vertex.normal.axis[0], vertex.normal.axis[1], vertex.normal.axis[2]))) ^
//				(hash<glm::vec4>()(glm::vec4(vertex.colour.axis[0], vertex.colour.axis[1], vertex.colour.axis[2], vertex.colour.axis[3])) << 1)) >> 1) ^
//				(hash<glm::vec2>()(glm::vec2(vertex.texCoord.axis[0], vertex.texCoord.axis[1])) << 1);
//		}
//	};
//}


namespace std {
	template<> struct hash<Vertex> {
		size_t operator()(Vertex const& vertex) const {
			return ((hash<glm::vec3>()(glm::vec3(vertex.pos.axis[0], vertex.pos.axis[1], vertex.pos.axis[2])) ^
				(hash<glm::vec4>()(glm::vec4(vertex.colour.axis[0], vertex.colour.axis[1], vertex.colour.axis[2], vertex.colour.axis[3])) << 1)) >> 1) ^
				(hash<glm::vec2>()(glm::vec2(vertex.texCoord.axis[0], vertex.texCoord.axis[1])) << 1);
		}
	};
}