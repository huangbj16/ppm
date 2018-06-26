#pragma once
#include "vec3.h"
#include "color.h"
#include "camera.h"
#include "light.h"
#include "thing.h"

class HitPoint {
public:
	Vec3 point, vecN, view_dir;
	int imagex, imagey;
	double weight, radius;
	int count;
	Color colorflux;
	HitPoint *next;
	HitPoint();
	void Update(Vec3 crash_point, Vec3 dir, Color color);
};

class HitPointList {
public:
	HitPoint *list;
	int hashvalue;
	Color photonimage[1000][1000];
	HitPointList() {}
	HitPointList(int _hashvalue);
	void AddHitPoint(HitPoint *hp);
	int Hash(Vec3 point);
	void AddPhoton(Vec3 point, Vec3 dir, Color color);
	void CalculatePhotonImage();
};

class Raytracer {
public:
	Color image[1000][1000];
	Color photonimage[1000][1000];
	int n, m;
	Camera *camera;
	Light **light;
	int lightnum;
	Thing **thing;
	int thingnum;
	HitPointList *hplist;
	
	Raytracer(int _n, int _m, int _lightnum, int _thingnum, int _hashvalue);
	void Calculate();
	Color Intersect(Line line, int time, int i, int j, double weight);
	Color CalculateColor(CrashBag bag, Vec3 view_direction, Thing *crash_thing, Pointlight *crash_light, int p);
	void ShootPhoton();
	void PhotonIntersect(Line line, Color light, int time, double depress);
};