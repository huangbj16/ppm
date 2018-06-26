#include "thing.h"
#include <Eigen/Dense>

bool Thing::Inside(Vec3 point)
{
	return false;
}

double Thing::LengthInside(Vec3 vecN, Vec3 dir)
{
	return 10000.0f;
}

Ball::Ball(Vec3 _c, double _r, Material _m)
{
	center = _c;
	radius = _r;
	material = _m;
}

CrashBag Ball::Crash(Vec3 source, Vec3 dir)
{
	Vec3 l = center - source;
	if (l.Module() > radius) {//outside the ball;
		double tp = l.Dot(dir);
		if (tp > 1e-6) {//cross maybe
			double d = sqrt(l.Module2() - tp*tp);
			if (d < radius) {//cross
				double r = sqrt(radius*radius - d*d);
				double distance = tp - r;
				CrashBag bag;
				bag.crash_point = source + dir*distance;
				bag.vecN = (bag.crash_point - center).GetUnitVector();
				return bag;
			}
		}
	}
	else if(l.Module() < radius){//inside the ball;
		double LCos = l.Dot(dir.GetUnitVector());
		double dist = sqrt(radius*radius - l.Module2() + LCos*LCos) + LCos;
		CrashBag bag;
		bag.crash_point = source + dir*dist;
		bag.vecN = (center - bag.crash_point).GetUnitVector();
		return bag;
	}
	return CrashBag();
}
/*
Vec3 Ball::GetvecN(Vec3 * crash_point)
{
	return (*crash_point - center).GetUnitVector();
}
*/
bool Ball::Inside(Vec3 point)
{
	double dist = (center - point).Module();
	if (dist < radius) return true;
	else return false;
}

double Ball::LengthInside(Vec3 vecN, Vec3 dir)
{
	double dot = -vecN.Dot(dir.GetUnitVector());
	if (dot > 0) {
		double CosI = sqrt(dot);
		return 2 * radius * CosI;
	}
	else return 0;
}

Plain::Plain()
{
}

Plain::Plain(Vec3 _vecN, double _offset, Material _m)
{
	vecN = _vecN;
	offset = _offset;
	material = _m;
}

CrashBag Plain::Crash(Vec3 source, Vec3 dir)
{
	double down = vecN.Dot(dir);
	//printf("%lf ", down);
	if (fabs(down) >= EPS) {
		double t = -(offset + (vecN.Dot(source))) / down;
		if (t > EPS) {
			CrashBag bag;
			bag.crash_point = source + dir*t;
			bag.vecN = vecN;
			return bag;
		}
	}
	return CrashBag();
}
/*
Vec3 Plain::GetvecN(Vec3 * crash_point)
{
	return vecN;
}
*/
Bezier::Bezier(std::string filename, Material _m)
{
	n = 3;
	m = 3;
	material = _m;
	mincover.x = 100;
	mincover.y = 100;
	mincover.z = 100;
	maxcover.x = -100;
	maxcover.y = -100;
	maxcover.z = -100;
	std::ifstream fin(filename);
	for (int i = 0; i < n + 1; ++i) {
		for (int j = 0; j < m + 1; ++j) {
			fin>>control[i][j].x >> control[i][j].y >> control[i][j].z;
			if (mincover.x > control[i][j].x) mincover.x = control[i][j].x;
			if (mincover.y > control[i][j].y) mincover.y = control[i][j].y;
			if (mincover.z > control[i][j].z) mincover.z = control[i][j].z;
			if (maxcover.x < control[i][j].x) maxcover.x = control[i][j].x;
			if (maxcover.y < control[i][j].y) maxcover.y = control[i][j].y;
			if (maxcover.z < control[i][j].z) maxcover.z = control[i][j].z;
		}
	}
	cover[0] = Plain(Vec3(1, 0, 0), -mincover.x, Material());
	cover[1] = Plain(Vec3(1, 0, 0), -maxcover.x, Material());
	cover[2] = Plain(Vec3(0, 1, 0), -mincover.y, Material());
	cover[3] = Plain(Vec3(0, 1, 0), -maxcover.y, Material());
	cover[4] = Plain(Vec3(0, 0, 1), -mincover.z, Material());
	cover[5] = Plain(Vec3(0, 0, 1), -maxcover.z, Material());
	Initial();
}

void Bezier::Initial()
{
	int up = 1, down = 1, temp = 1;
	Bx[0] = Bx[n] = 1;
	for (int i = 1; i <= (n / 2); ++i) {
		temp = temp * (n - i + 1) / i;
		Bx[i] = Bx[n - i] = temp;
	}

	up = down = temp = 1;
	By[0] = By[m] = 1;
	for (int i = 1; i <= (m / 2); ++i) {
		temp = temp * (m - i + 1) / i;
		By[i] = By[m - i] = temp;
	}
	
	factor[0][0] = 1;
	factor[0][1] = -3;
	factor[0][2] = 3;
	factor[0][3] = -1;
	factor[1][0] = 0;
	factor[1][1] = 1;
	factor[1][2] = -2;
	factor[1][3] = 1;
	factor[2][0] = 0;
	factor[2][1] = 0;
	factor[2][2] = 1;
	factor[2][3] = -1;
	factor[3][0] = 0;
	factor[3][1] = 0;
	factor[3][2] = 0;
	factor[3][3] = 1;

	dfactor[0][0] = -3;
	dfactor[0][1] = 6;
	dfactor[0][2] = -3;
	dfactor[1][0] = 1;
	dfactor[1][1] = -4;
	dfactor[1][2] = 3;
	dfactor[2][0] = 0;
	dfactor[2][1] = 2;
	dfactor[2][2] = -3;
	dfactor[3][0] = 0;
	dfactor[3][1] = 0;
	dfactor[3][2] = 3;
}

CrashBag Bezier::Crash(Vec3 source, Vec3 dir)
{
	//四个bug解决：inverse不会改变dF，需要重新赋值；uvt顺序出现问题；rand是整数需要转换；u，v可以多次赋初值。
	Vec3 crash_point = Crash_cover(source, dir);
	if (crash_point.IsNullVector()) return CrashBag();//not crashed with the cover, impossible with the bezier
	
	double u, v, t;//牛顿迭代初始值
	double t0 = (crash_point - source).Module();
	Vec3 temp, tempu, tempv;
	Eigen::Matrix3d dF, dFinverse;
	Eigen::Vector3d p, pnext, F;
	dF(0, 2) = -dir.x;
	dF(1, 2) = -dir.y;
	dF(2, 2) = -dir.z;
	for (int q = 0; q < 10; ++q) {
		u = (double)rand() / RAND_MAX;
		v = (double)rand() / RAND_MAX;
		t = t0;
		crash_point = source + dir*t;
		for (int i = 0; i < 10; ++i) {
			p << u, v, t;
			temp = GetPoint(u, v) - crash_point;
			//printf("%d: %lf %lf %lf %lf\n", i, u, v, t, temp.Module());
			F << temp.x, temp.y, temp.z;
			tempu = GetDiffU(u, v);
			tempv = GetDiffV(u, v);
			dF(0, 0) = tempu.x;
			dF(1, 0) = tempu.y;
			dF(2, 0) = tempu.z;
			dF(0, 1) = tempv.x;
			dF(1, 1) = tempv.y;
			dF(2, 1) = tempv.z;
			//printf("%lf %lf %lf %lf %lf %lf\n", dF(0, 0), dF(1, 0), dF(2, 0), dF(0, 1), dF(1, 1), dF(2, 1));
			dFinverse = dF.inverse();
			//printf("%lf %lf %lf %lf %lf %lf\n", dFinverse(0, 0), dFinverse(1, 0), dFinverse(2, 0), dFinverse(0, 1), dFinverse(1, 1), dFinverse(2, 1));
			pnext = p - dFinverse * F;
			u = pnext(0);
			v = pnext(1);
			t = pnext(2);
			crash_point = source + dir*t;
			if (temp.IsZeroVector() && u >= 0 && u <= 1 && v >= 0 && v <= 1) {
				//printf("u, v, delta: %lf, %lf %lf\n", u, v, temp.Module());
				CrashBag bag;
				bag.crash_point = crash_point;
				Vec3 cross = (tempu*tempv).GetUnitVector();
				if (cross.Dot(dir) < 0) bag.vecN = cross;
				else bag.vecN = cross*(-1);
				bag.u = u;
				bag.v = v;
				return bag;
			}
		}
	}
	return CrashBag();
}
/*
Vec3 Bezier::GetvecN(Vec3 * crash_point)
{
	if (((*crash_point) - precrash).IsZeroVector()) {
		Vec3 diffu = GetDiffU(preu, prev);
		Vec3 diffv = GetDiffV(preu, prev);
		Vec3 cross = (diffu*diffv).GetUnitVector();
		Vec3 dir = (*crash_point) - Vec3(0, 10, 0);
		
		if (cross.Dot(dir) < 0) {
			return cross;
		}
		else {
			return cross*(-1);
		}

		cross.Print();
		return cross;
	}
	printf("else\n");
	return Vec3(0, 0, 0);
}
*/
Vec3 Bezier::GetPoint(double u, double v)
{
	Vec3 p(0, 0, 0);
	double t1;
	for (int i = 0; i < n+1; ++i) {
		for (int j = 0; j < m + 1; ++j) {
			t1 = Bx[i] * By[j] * (factor[i][0] + factor[i][1] * u + factor[i][2] * u*u + factor[i][3] * u*u*u) * (factor[j][0] + factor[j][1] * v + factor[j][2] * v*v + factor[j][3] * v*v*v);
			p.x += t1 * control[i][j].x;
			p.y += t1 * control[i][j].y;
			p.z += t1 * control[i][j].z;
		}
	}
	return p;
}

Vec3 Bezier::GetDiffU(double u, double v)
{
	Vec3 p(0, 0, 0);
	double t1;
	for (int i = 0; i < n + 1; ++i) {
		for (int j = 0; j < m + 1; ++j) {
			t1 = Bx[i] * By[j] * (dfactor[i][0] + dfactor[i][1] * u + dfactor[i][2] * u*u) * (factor[j][0] + factor[j][1] * v + factor[j][2] * v*v + factor[j][3] * v*v*v);
			p.x += t1 * control[i][j].x;
			p.y += t1 * control[i][j].y;
			p.z += t1 * control[i][j].z;
		}
	}
	return p;
}

Vec3 Bezier::GetDiffV(double u, double v)
{
	Vec3 p(0, 0, 0);
	double t1;
	for (int i = 0; i < n + 1; ++i) {
		for (int j = 0; j < m + 1; ++j) {
			t1 = Bx[i] * By[j] * (factor[i][0] + factor[i][1] * u + factor[i][2] * u*u + factor[i][3] * u*u*u) * (dfactor[j][0] + dfactor[j][1] * v + dfactor[j][2] * v*v);
			p.x += t1 * control[i][j].x;
			p.y += t1 * control[i][j].y;
			p.z += t1 * control[i][j].z;
		}
	}
	return p;
}



Vec3 Bezier::Crash_cover(Vec3 source, Vec3 dir)
{
	Vec3 crash_point;
	crash_point = cover[0].Crash(source, dir).crash_point;
	if (!crash_point.IsNullVector()) {
		if (crash_point.y > mincover.y && crash_point.y < maxcover.y && crash_point.z > mincover.z && crash_point.z < maxcover.z)
			return crash_point;
	}
	crash_point = cover[1].Crash(source, dir).crash_point;
	if (!crash_point.IsNullVector()) {
		if (crash_point.y > mincover.y && crash_point.y < maxcover.y && crash_point.z > mincover.z && crash_point.z < maxcover.z)
			return crash_point;
	}
	crash_point = cover[2].Crash(source, dir).crash_point;
	if (!crash_point.IsNullVector()) {
		if (crash_point.x > mincover.x && crash_point.x < maxcover.x && crash_point.z > mincover.z && crash_point.z < maxcover.z)
			return crash_point;
	}
	crash_point = cover[3].Crash(source, dir).crash_point;
	if (!crash_point.IsNullVector()) {
		if (crash_point.x > mincover.x && crash_point.x < maxcover.x && crash_point.z > mincover.z && crash_point.z < maxcover.z)
			return crash_point;
	}
	crash_point = cover[4].Crash(source, dir).crash_point;
	if (!crash_point.IsNullVector()) {
		if (crash_point.x > mincover.x && crash_point.x < maxcover.x && crash_point.y > mincover.y && crash_point.y < maxcover.y)
			return crash_point;
	}
	crash_point = cover[5].Crash(source, dir).crash_point;
	if (!crash_point.IsNullVector()) {
		if (crash_point.x > mincover.x && crash_point.x < maxcover.x && crash_point.y > mincover.y && crash_point.y < maxcover.y)
			return crash_point;
	}
	return Vec3(10000,10000,10000);
}

CrashBag::CrashBag()
{
	crash_point = Vec3(10000, 10000, 10000);
	vecN = Vec3(0, 0, 0);
	u = 0;
	v = 0;
}
