#include "material.h"

Material::Material() :color(Color(0.2f, 0.2f, 0.2f)), reflection(0), diffusion(0.2f), specular(0.8f), refraction(0), refraction_index(0){}

Material::Material(Color _c, double _rfl, double _d, double _s, double _rfr, double _rfrindex)
{
	color = _c;
	reflection = _rfl;
	refraction = _rfr;
	refraction_index = _rfrindex;
	diffusion = _d;
	specular = _s;
}
