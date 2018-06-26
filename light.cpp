#include "light.h"
//old version
/*
Color Pointlight::CalculateColor(Vec3 point, Vec3 dir, Vec3 vecN)//phongÄ£ÐÍ
{
	Vec3 l = (pos - point).GetUnitVector();
	Vec3 rdir = Vec3(-dir.x, -dir.y, -dir.z).GetUnitVector();
	vecN = vecN.GetUnitVector();
	Vec3 h = (dir + l) / 2;
	double res = vecN.Dot(h);
	if (res <= 0) {
		res = 0 + roud;
	}
	else {
		res = roud + rous * pow(res, 20) / vecN.Dot(l);
	}
	return Color(255, 255, 255) * res * 0.8;
}
*/

Pointlight::Pointlight(Material _m, Vec3 _pos)
{
	material = _m;
	pos = _pos;
}

Line Pointlight::GeneratePhoton()
{
	Line line;
	line.point = pos;
	line.dir = Vec3::RandUnitVector();
	return line;
}
