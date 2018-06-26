#pragma once
#include "color.h"

class Material {
public:
	Color color;
	double reflection;

	double refraction;
	double refraction_index;

	double diffusion;
	double specular;
	Material();
	Material(Color _c, double _rfl, double _d, double _s, double _rfr, double _rfrindex);
};