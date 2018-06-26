#include"vec3.h"
#include<cmath>
#include<cstdlib>
#define ran() ( double( rand() % RAND_MAX ) / RAND_MAX )

const double EPS = 1e-6;
const double PI = 3.1415926535897932384626;

Vec3 operator + (const Vec3& A, const Vec3& B) {
	return Vec3(A.x + B.x, A.y + B.y, A.z + B.z);
}

Vec3 operator - (const Vec3& A, const Vec3& B) {
	return Vec3(A.x - B.x, A.y - B.y, A.z - B.z);
}

Vec3 operator * (const Vec3& A, const double& k) {
	return Vec3(A.x * k, A.y * k, A.z * k);
}

Vec3 operator / (const Vec3& A, const double& k) {
	return Vec3(A.x / k, A.y / k, A.z / k);
}

Vec3 operator * (const Vec3& A, const Vec3& B) {
	return Vec3(A.y * B.z - A.z * B.y, A.z * B.x - A.x * B.z, A.x * B.y - A.y * B.x);
}

Vec3& operator += (Vec3& A, const Vec3& B) {
	A = A + B;
	return A;
}

Vec3& operator -= (Vec3& A, const Vec3& B) {
	A = A - B;
	return A;
}

Vec3& operator *= (Vec3& A, const double& k) {
	A = A * k;
	return A;
}

Vec3& operator += (Vec3& A, const double& k) {
	A = A / k;
	return A;
}

Vec3& operator *= (Vec3& A, const Vec3& B) {
	A = A * B;
	return A;
}

Vec3 operator - (const Vec3& A) {
	return Vec3(-A.x, -A.y, -A.z);
}

double Vec3::Dot(const Vec3& term) {
	return x * term.x + y * term.y + z * term.z;
}

double Vec3::Module2() {
	return x * x + y * y + z * z;
}

double Vec3::Module() {
	return sqrt(x * x + y * y + z * z);
}

double Vec3::Distance2(Vec3& term) {
	return (term - *this).Module2();
}

double Vec3::Distance(Vec3& term) {
	return (term - *this).Module();
}

double& Vec3::GetCoord(int axis) {
	if (axis == 0) return x;
	if (axis == 1) return y;
	if (axis == 2) return z;
}

Vec3 Vec3::GetUnitVector() {
	return *this / Module();
}

void Vec3::AssRandomVector() {
	do {
		x = 2 * ran() - 1;
		y = 2 * ran() - 1;
		z = 2 * ran() - 1;
	} while (x * x + y * y + z * z > 1 || x * x + y * y + z * z < EPS);
	*this = GetUnitVector();
}

Vec3 Vec3::GetAnVerticalVector() {
	Vec3 ret = *this * Vec3(0, 0, 1);
	if (ret.IsZeroVector()) ret = Vec3(1, 0, 0);
	else ret = ret.GetUnitVector();
	return ret;
}

bool Vec3::IsZeroVector() {
	return fabs(x) < EPS && fabs(y) < EPS && fabs(z) < EPS;
}

bool Vec3::IsNullVector()
{
	return ((*this) - Vec3(10000, 10000, 10000)).IsZeroVector();
}

void Vec3::Input(std::stringstream& fin) {
	fin >> x >> y >> z;
}

Vec3 Vec3::Reflect(Vec3 N) {
	return *this - N * (2 * Dot(N));
}

Vec3 Vec3::Refract(Vec3 N, double n) {
	Vec3 V = GetUnitVector();
	double cosI = -N.Dot(V), cosT2 = 1 - (n * n) * (1 - cosI * cosI);
	if (cosT2 > EPS) return V * n + N * (n * cosI - sqrt(cosT2));
	else return Vec3(0, 0, 0);
}

Vec3 Vec3::Diffuse() {
	Vec3 Vert = GetAnVerticalVector();
	double theta = acos(sqrt(ran()));
	double phi = ran() * 2 * PI;
	return Rotate(Vert, theta).Rotate(*this, phi);
}

Vec3 Vec3::Rotate(Vec3 axis, double theta) {
	Vec3 ret;
	double cost = cos(theta);
	double sint = sin(theta);
	ret.x += x * (axis.x * axis.x + (1 - axis.x * axis.x) * cost);
	ret.x += y * (axis.x * axis.y * (1 - cost) - axis.z * sint);
	ret.x += z * (axis.x * axis.z * (1 - cost) + axis.y * sint);
	ret.y += x * (axis.y * axis.x * (1 - cost) + axis.z * sint);
	ret.y += y * (axis.y * axis.y + (1 - axis.y * axis.y) * cost);
	ret.y += z * (axis.y * axis.z * (1 - cost) - axis.x * sint);
	ret.z += x * (axis.z * axis.x * (1 - cost) - axis.y * sint);
	ret.z += y * (axis.z * axis.y * (1 - cost) + axis.x * sint);
	ret.z += z * (axis.z * axis.z + (1 - axis.z * axis.z) * cost);
	return ret;
}

void Vec3::Print() {
	printf("%lf %lf %lf\n", x, y, z);
}

Vec3 Vec3::RandUnitVector()
{
	double theta, phi;
	theta = ran() * 2 * PI;
	phi = ran() * 2 * PI;
	Vec3 r;
	r.x = cos(theta);
	r.y = sin(theta) * cos(phi);
	r.z = sin(theta) * sin(phi);
	return r.GetUnitVector();
}
