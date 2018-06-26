#pragma once
#include <cstdio>
#include <iostream>
#include "vec3.h"
using namespace std;

class BezierCurve {
private:
	Vec3 *point;
	int n;
public:
	BezierCurve(int _n, Vec3 *p);
	Vec3 GetPoint(double t);
};