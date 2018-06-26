#pragma once
#include <cstdio>
#include <iostream>
#include <cmath>
#include "vec3.h"
using namespace std;

class BezierSurface {
private:
	Vec3 **point;
	int n, m;
	int *Bx, *By;
public:
	BezierSurface(int _n, int _m, Vec3 p[][4]);//第二维锁定了！！
	void InitialB();
	Vec3 GetPoint(double u, double v);
};