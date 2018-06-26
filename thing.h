#pragma once
#include "vec3.h"
#include "dir2.h"
#include "material.h"
#include <cmath>
#include <stdio.h>
#include <fstream>
#include <string>

class CrashBag {
public:
	Vec3 crash_point;
	Vec3 vecN;
	double u, v;
	CrashBag();
};

class Thing {
public:
	Material material;

	virtual CrashBag Crash(Vec3 source, Vec3 dir) = 0;
	//virtual Vec3 GetvecN(Vec3 *crash_point) = 0;
	virtual bool Inside(Vec3 point);
	virtual double LengthInside(Vec3 vecN, Vec3 dir);
};

class Ball : public Thing {
public:
	double radius;
	Vec3 center;
	Ball(Vec3 _c, double _r, Material _m);
	CrashBag Crash(Vec3 source, Vec3 dir);
	//Vec3 GetvecN(Vec3 *crash_point);
	bool Inside(Vec3 point);
	double LengthInside(Vec3 vecN, Vec3 dir);
};

class Plain : public Thing {
private://vecN * X + offset = 0
	double offset;
	Vec3 vecN;
public:
	Plain();
	Plain(Vec3 _vecN, double _offset, Material _m);
	CrashBag Crash(Vec3 source, Vec3 dir);
	//Vec3 GetvecN(Vec3 *crash_point);
};

class Bezier : public Thing {
public:
	Vec3 control[4][4];//4*4¿ØÖÆµã
	int n, m;//fixed 3 3
	int Bx[4], By[4];
	double factor[4][4];
	double dfactor[4][3];
	Vec3 mincover, maxcover;
	//Vec3 precrash;
	//double preu, prev;
	Plain cover[6];
	Bezier(std::string filename, Material _m);
	void Initial();
	CrashBag Crash(Vec3 source, Vec3 dir);
	//Vec3 GetvecN(Vec3 *crash_point);
	Vec3 GetPoint(double u, double v);
	Vec3 GetDiffU(double u, double v);
	Vec3 GetDiffV(double u, double v);
	Vec3 Crash_cover(Vec3 source, Vec3 dir);
};