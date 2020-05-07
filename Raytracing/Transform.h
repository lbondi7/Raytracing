#pragma once

#include "Vec3.h"

struct Transform
{
	Vec3 position;
	Vec3 size;

	void operator=(const Transform& t) { 
		position = t.position; 
		size = t.size;
	}

};