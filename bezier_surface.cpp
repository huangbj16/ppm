#include "bezier_surface.h"

BezierSurface::BezierSurface(int _n, int _m, Vec3 p[][4])
{
	n = _n;
	m = _m;
	point = new Vec3*[n+1];
	for (int i = 0; i < n+1; ++i) {
		point[i] = new Vec3[m+1];
		for (int j = 0; j < m+1; ++j) {
			point[i][j] = p[i][j];
		}
	}
	InitialB();
}

void BezierSurface::InitialB()
{
	Bx = new int[n + 1];
	By = new int[m + 1];
	
	int up = 1, down = 1, temp = 1;
	Bx[0] = Bx[n] = 1;
	for (int i = 1; i <= (n / 2); ++i) {
		temp = temp * (n - i + 1) / i;
		Bx[i] = Bx[n - i] = temp;
	}

	up = 1;
	down = 1;
	temp = 1;
	By[0] = By[m] = 1;
	for (int i = 1; i <= (m / 2); ++i) {
		temp = temp * (m - i + 1) / i;
		By[i] = By[m - i] = temp;
	}
	/*
	for (int i = 0; i < n + 1; ++i) {
		cout << Bx[i] << endl;
	}
	for (int i = 0; i < m + 1; ++i) {
		cout << By[i] << endl;
	}
	*/
}

Vec3 BezierSurface::GetPoint(double u, double v)
{
	Vec3 p;
	p.x = 0;
	p.y = 0;
	p.z = 0;
	for (int i = 0; i < n + 1; ++i) {
		for (int j = 0; j < m + 1; ++j) {
			p.x += Bx[i] * pow(u, i)*pow(1 - u, n - i)*By[j] * pow(v, j)*pow(1 - v, m - j)*point[i][j].x;
			p.y += Bx[i] * pow(u, i)*pow(1 - u, n - i)*By[j] * pow(v, j)*pow(1 - v, m - j)*point[i][j].y;
			p.z += Bx[i] * pow(u, i)*pow(1 - u, n - i)*By[j] * pow(v, j)*pow(1 - v, m - j)*point[i][j].z;
		}
	}
	return p;
}
