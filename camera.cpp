#include "camera.h"

Line::Line(Vec3 _point, Vec3 _dir)
{
	point = _point;
	dir = _dir;
}

Camera::Camera(Vec3 _viewpoint, int _n, int _m, Vec3 ** _viewplain)
{
	viewpoint = _viewpoint;
	n = _n;
	m = _m;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			viewplain[i][j] = _viewplain[i][j];
		}
	}
	CalculateLine();
}

void Camera::CalculateLine()
{
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			viewlines[i][j].point = viewpoint;
			viewlines[i][j].dir = (viewplain[i][j] - viewpoint).GetUnitVector();
		}
	}
}

