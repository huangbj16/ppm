#ifndef VEC3_H
#define VEC3_H

#include<sstream>

extern const double EPS;
extern const double PI;

class Vec3 {
public:
	double x, y, z;

	Vec3(double X = 0, double Y = 0, double Z = 0) : x(X), y(Y), z(Z) {}
	~Vec3() {}

	friend Vec3 operator + (const Vec3&, const Vec3&);
	friend Vec3 operator - (const Vec3&, const Vec3&);
	friend Vec3 operator * (const Vec3&, const double&);
	friend Vec3 operator / (const Vec3&, const double&);
	friend Vec3 operator * (const Vec3&, const Vec3&); //cross product
	friend Vec3& operator += (Vec3&, const Vec3&);
	friend Vec3& operator -= (Vec3&, const Vec3&);
	friend Vec3& operator *= (Vec3&, const double&);
	friend Vec3& operator /= (Vec3&, const double&);
	friend Vec3& operator *= (Vec3&, const Vec3&);
	friend Vec3 operator - (const Vec3&);
	double Dot(const Vec3&);
	double Module2();
	double Module();
	double Distance2(Vec3&);
	double Distance(Vec3&);
	double& GetCoord(int axis);
	Vec3 GetUnitVector();
	void AssRandomVector();
	Vec3 GetAnVerticalVector();
	bool IsZeroVector();
	bool IsNullVector();
	void Input(std::stringstream& fin);
	Vec3 Reflect(Vec3 N);
	Vec3 Refract(Vec3 N, double n);
	Vec3 Diffuse();
	Vec3 Rotate(Vec3 axis, double theta);
	void Print();
	static Vec3 RandUnitVector();
};

#endif