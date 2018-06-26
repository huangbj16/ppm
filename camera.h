#pragma once
#include "vec3.h"
#include "line.h"

class Camera {
public:
	Vec3 viewpoint;
	int n, m;
	Vec3 viewplain[1000][1000];//fixed
	Line viewlines[1000][1000];

	Camera(Vec3 _viewpoint, int _n, int _m, Vec3 **_viewplain);
	void CalculateLine();
};

