#include "bezier_curve.h"

BezierCurve::BezierCurve(int _n, Vec3 *p)
{
	n = _n;
	point = new Vec3[n];
	for (int i = 0; i < n; ++i) {
		point[i].x = p[i].x;
		point[i].y = p[i].y;
		point[i].z = p[i].z;
		//cout << point[i].x << " " << point[i].y << endl;
	}
}

Vec3 BezierCurve::GetPoint(double t)
{
	Vec3 *p = new Vec3[n];
	for (int i = 0; i < n; ++i)
		p[i] = point[i];
	for (int i = 1; i < n; ++i) {
		for (int j = 0; j < n - i; ++j) {
			p[j].x = (1.0 - t)*p[j].x + t*p[j+1].x;
			p[j].y = (1.0 - t)*p[j].y + t*p[j + 1].y;
			p[j].z = (1.0 - t)*p[j].z + t*p[j + 1].z;
		}
	}
	Vec3 _p = p[0];
	delete []p;
	return _p;
}
