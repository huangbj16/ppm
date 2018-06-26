#pragma once
#include "vec3.h"

class Line {
public:
	Vec3 point, dir;

	Line(Vec3 _point, Vec3 _dir);
	Line() {}
};