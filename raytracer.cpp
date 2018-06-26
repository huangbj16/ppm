#include "raytracer.h"
#include <Windows.h>
#include <omp.h>
#define ran() ( double( rand() % RAND_MAX ) / RAND_MAX )


HitPoint::HitPoint() {
	colorflux = Color(0, 0, 0);
	count = 0;
	next = NULL;
}

void HitPoint::Update(Vec3 crash_point, Vec3 dir, Color color)
{
	if ((crash_point - point).Module() > radius) return;
	double dot = dir.Dot(vecN) * (-1);
	if (dot > 0) {
		colorflux += color * dot;
	}
	count++;
}

HitPointList::HitPointList(int _hashvalue)
{
	hashvalue = _hashvalue;
	list = new HitPoint[hashvalue];
}

void HitPointList::AddHitPoint(HitPoint *hp)
{
	int hash = Hash(hp->point);
	//hp->point.Print();
	//printf("hash: %d\n", hash);
	HitPoint *p = list[hash].next;
	if (p != NULL) {
		while (p->next != NULL) {
			p = p->next;
		}
		p->next = hp;
	}
	else {
		list[hash].next = hp;
	}
}

int HitPointList::Hash(Vec3 point)
{
	int x = point.x * 10;
	int y = point.y * 10;
	int z = point.z * 10;
	return (unsigned int)((x * 73856093) ^ (y * 19349663) ^ (z * 83492791)) % hashvalue;
}

void HitPointList::AddPhoton(Vec3 point, Vec3 dir, Color color)
{
	int hash = Hash(point);
	HitPoint *p = &list[hash];
	while (p != NULL) {
		p->Update(point, dir, color);
		p = p->next;
	}
}

void HitPointList::CalculatePhotonImage()
{
	int x, y;
	Color contribute;
	for (int i = 0; i < hashvalue; ++i) {
		HitPoint *p = list[i].next;
		while (p != NULL) {
			if (p->count == 0) {
				p = p->next;
				continue;
			}
			contribute = p->colorflux / p->count;
			x = p->imagex;
			y = p->imagey;
			photonimage[x][y] += contribute * p->weight;
			p->radius *= 0.99;
			p = p->next;
		}
	}
}

Raytracer::Raytracer(int _n, int _m, int _lightnum, int _thingnum, int _hashvalue)
{
	n = _n;
	m = _m;
	lightnum = _lightnum;
	thingnum = _thingnum;
	light = new Light*[lightnum];
	thing = new Thing*[thingnum];
	hplist = new HitPointList(_hashvalue);
}

void Raytracer::Calculate()
{
#pragma omp parallel for num_threads(8)
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			Line line = camera->viewlines[i][j];
			Color c = Intersect(line, 0, i, j, 1);
			image[i][j] = c;
		}
	}
	printf("ray tracing finished\n");
}

Color Raytracer::Intersect(Line line, int time, int i, int j, double weight) {
	if (time == 5) {
		return Color(0, 0, 0);//enough!
	}
	CrashBag crash_bag;
	CrashBag bag;
	int p = -1;
	for (int i = 0; i < thingnum; ++i) {
		bag = thing[i]->Crash(line.point, line.dir);
		if (!bag.crash_point.IsNullVector()) {
			if (crash_bag.crash_point.Distance2(line.point) > bag.crash_point.Distance2(line.point)) {
				crash_bag = bag;
				p = i;
			}
		}
	}
	if (!crash_bag.crash_point.IsNullVector()) {
		
		HitPoint *hp = new HitPoint();
		hp->imagex = i;
		hp->imagey = j;
		hp->point = crash_bag.crash_point;
		hp->radius = 0.01;
		hp->vecN = crash_bag.vecN;
		hp->view_dir = line.dir;
		hp->weight = weight;
		hplist->AddHitPoint(hp);
		
		Color colorlight(0, 0, 0);
		if (!thing[p]->Inside(line.point)) {//outside
			Vec3 check(-9.96, -4.4, 7.5);
			for (int i = 0; i < lightnum; ++i) {
				colorlight += CalculateColor(crash_bag, line.dir, thing[p], (Pointlight*)light[i], p);//calculate color at the point
			}
			Vec3 vecN = crash_bag.vecN;
			if (thing[p]->material.reflection > 0) {
				Line newline;
				newline.dir = line.dir.Reflect(vecN);
				newline.point = bag.crash_point + newline.dir * EPS;
				colorlight += Intersect(newline, time + 1, i, j, weight * thing[p]->material.reflection) * thing[p]->material.color * thing[p]->material.reflection;//calculate reflect color
			}
			if (thing[p]->material.refraction > 0) {//calculate refraction color
				double n = thing[p]->material.refraction_index;
				Line newline;
				n = 1 / n;
				newline.dir = line.dir.Refract(vecN, n);
				if (!newline.dir.IsZeroVector()) {
					newline.point = crash_bag.crash_point + newline.dir * EPS;
					double absorb = exp((-0.01f) * thing[p]->LengthInside(vecN, newline.dir));
					colorlight += Intersect(newline, time + 1, i, j, weight * thing[p]->material.refraction * absorb) * (thing[p]->material.refraction * absorb);
				}
			}
		}
		else {//inside
			Vec3 vecN = crash_bag.vecN;
			double n = thing[p]->material.refraction_index;
			Line newline;
			newline.dir = line.dir.Refract(vecN, n);
			if (!newline.dir.IsZeroVector()) {
				newline.point = crash_bag.crash_point + newline.dir * EPS;
				colorlight += Intersect(newline, time + 1, i, j, weight * thing[p]->material.refraction) * thing[p]->material.refraction;
			}
		}
		return colorlight;
	}
	else {//no crash
		return Color(0, 0, 0);
	}
}
Color Raytracer::CalculateColor(CrashBag bag, Vec3 view_direction, Thing *crash_thing, Pointlight *crash_light, int p) {
	Vec3 crash_point = bag.crash_point;
	Vec3 l = (crash_light->pos - crash_point).GetUnitVector();
	Vec3 vecN = bag.vecN;
	Vec3 r = l.Reflect(vecN);
	Color c(0, 0, 0);
	//first calculate shadow;
	double dist = l.Module2();
	CrashBag other_bag;
	for (int i = 0; i < thingnum; ++i) {
		if (i == p) continue;
		other_bag = thing[i]->Crash(crash_point, l);
		if (!other_bag.crash_point.IsNullVector()) {
			if ((other_bag.crash_point - crash_point).Module2() < dist) {
				return Color(0, 0, 0);
			}
		}
	}
	//no shadow, calculate diffusion and speculation
	if (crash_thing->material.diffusion > 0) {
		double dot = l.Dot(vecN);
		if (dot > 0) {
			double diff = dot * crash_thing->material.diffusion;
			c += crash_thing->material.color * crash_light->material.color * diff;
		}
	}
	if (crash_thing->material.specular > 0) {
		double dot = r.Dot(view_direction);
		if (dot > 0) {
			c += crash_light->material.color * crash_thing->material.specular * pow(dot, 20);
		}
	}
	return c;
}

void Raytracer::ShootPhoton()
{
	for (int i = 0; i < lightnum; ++i) {
		#pragma omp parallel num_threads(8) schedule(dynamic, 1)
		for (int j = 0; j < 1000000; ++j) {
			Line photon = light[i]->GeneratePhoton();
			PhotonIntersect(photon, light[i]->material.color, 0, 0.9);
		}
	}
	printf("photon tracing finished\n");
}

void Raytracer::PhotonIntersect(Line line, Color light, int time, double depress) {
	if (time == 5) {
		return;//enough!
	}
	CrashBag crash_bag;
	CrashBag bag;
	int p = -1;
	for (int i = 0; i < thingnum; ++i) {
		bag = thing[i]->Crash(line.point, line.dir);
		if (!bag.crash_point.IsNullVector()) {
			if (crash_bag.crash_point.Distance2(line.point) > bag.crash_point.Distance2(line.point)) {
				crash_bag = bag;
				p = i;
			}
		}
	}
	if (!crash_bag.crash_point.IsNullVector()) {
		Vec3 russian = Vec3(thing[p]->material.diffusion, thing[p]->material.reflection, thing[p]->material.refraction);
		if (russian.IsZeroVector()) return;
		russian = russian.GetUnitVector();
		russian.y += russian.x;
		russian.z += russian.y;
		double drand = ran();
		light = light * depress;
		light = light * thing[p]->material.color;
		if(time != 0) hplist->AddPhoton(crash_bag.crash_point, line.dir, light);
		if (drand < russian.x) {//diff
			Line newline;
			newline.dir = Vec3::RandUnitVector();
			newline.point = crash_bag.crash_point + newline.dir * EPS;
			PhotonIntersect(newline, light, time + 1, depress);
		}
		else if (drand < russian.y) {//reflection
			Line newline;
			newline.dir = crash_bag.vecN.Reflect(line.dir);
			newline.point = crash_bag.crash_point + newline.dir * EPS;
			PhotonIntersect(newline, light, time + 1, depress);
		}
		else {//refraction
			double n = thing[p]->material.refraction_index;
			Line newline;
			if (!thing[p]->Inside(line.point)) n = 1 / n;//inside, refraction = 1/refraction
			newline.dir = line.dir.Refract(crash_bag.vecN, n);
			if (!newline.dir.IsZeroVector()) {
				newline.point = crash_bag.crash_point + newline.dir * EPS;
				PhotonIntersect(newline, light, time + 1, depress);
			}
		}
	}
}