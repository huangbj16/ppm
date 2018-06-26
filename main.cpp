#include "drawline.h"
#include "bezier_curve.h"
#include "bezier_surface.h"
#include "raytracer.h"
#include <ctime>
#include <fstream>

#define MX 1000
#define MY 1000

Vec3 standard_point[100];
Vec3 standard_matrix[4][4];
int n = 1000, m = 1000;
int lightnum = 2, thingnum = 8;
Vec3 viewpoint;
Vec3 **viewplain;

Raytracer *tracer;

void InitialMatrix();
void InitialParameter();

int main() {
	srand((unsigned)time(NULL));
	/*
	/////////////////////bezier_curve;
	Mat M(MY, MX, CV_8UC3, Scalar::all(255));
	memset(standard_point, 0, sizeof(Vec3));
	standard_point[0].x = 50;
	standard_point[0].y = 500;
	standard_point[1].x = 200;
	standard_point[1].y = 50;
	standard_point[2].x = 612;
	standard_point[2].y = 81;
	
	standard_point[3].x = 890;
	standard_point[3].y = 700;
	standard_point[4].x = 500;
	standard_point[4].y = 500;
	
	for (int i = 0; i < n-1; ++i) {
		IntegerBresenhamline(standard_point[i].x, standard_point[i].y, standard_point[(i+1)%n].x, standard_point[(i+1)%n].y, M);
	}
	BezierCurve curve(n, standard_point);
	for (double t = 0; t < 1; t += 0.001) {
		Vec3 p = curve.GetPoint(t);
		//cout << p.x << " " << p.y << endl;
		drawpixel(p.x+0.5, p.y+0.5, M, 0);
	}
	imshow("original", M);
	imwrite("line1.png", M);
	*/

	/*
	////////////bezier_surface
	Mat M2(MY, MX, CV_8UC3, Scalar::all(255));
	InitialMatrix();
	
	//drawsurface
	BezierSurface surface(n, m, standard_matrix);
	for (double u = 0; u < 1; u += 0.005) {
		for (double v = 0; v < 1; v += 0.005) {
			Vec3 p = surface.GetPoint(u, v);
			//cout << p.x << " " << p.y << endl;
			drawpixel(p.x + 0.5, p.y + 0.5, M2);
		}
	}

	//drawcontrolline;
	for (int i = 0; i <= n; ++i) {
		for (int j = 0; j <= n - 1; ++j) {
			IntegerBresenhamline(standard_matrix[i][j].x, standard_matrix[i][j].y, standard_matrix[i][j + 1].x, standard_matrix[i][j + 1].y, M2);
		}
	}
	for (int i = 0; i <= n - 1; ++i) {
		for (int j = 0; j <= n; ++j) {
			IntegerBresenhamline(standard_matrix[i][j].x, standard_matrix[i][j].y, standard_matrix[i + 1][j].x, standard_matrix[i + 1][j].y, M2);
		}
	}

	imshow("original", M2);
	imwrite("surface.png", M2);
	waitKey(0);
	*/
	
	/*
	m_Primitive = new Primitive*[100];
	// ground plane
	m_Primitive[0] = new PlanePrim(vector3(0, 1, 0), 4.4f);
	m_Primitive[0]->SetName("plane");
	m_Primitive[0]->GetMaterial()->SetReflection(0);
	m_Primitive[0]->GetMaterial()->SetDiffuse(1.0f);
	m_Primitive[0]->GetMaterial()->SetColor(Color(0.4f, 0.3f, 0.3f));
	// big sphere
	m_Primitive[1] = new Sphere(vector3(1, -0.8f, 3), 2.5f);
	m_Primitive[1]->SetName("big sphere");
	m_Primitive[1]->GetMaterial()->SetReflection(0.6f);
	m_Primitive[1]->GetMaterial()->SetColor(Color(0.7f, 0.7f, 0.7f));
	// small sphere
	m_Primitive[2] = new Sphere(vector3(-5.5f, -0.5, 7), 2);
	m_Primitive[2]->SetName("small sphere");
	m_Primitive[2]->GetMaterial()->SetReflection(1.0f);
	m_Primitive[2]->GetMaterial()->SetDiffuse(0.1f);
	m_Primitive[2]->GetMaterial()->SetColor(Color(0.7f, 0.7f, 1.0f));
	// light source 1
	m_Primitive[3] = new Sphere(vector3(0, 5, 5), 0.1f);
	m_Primitive[3]->Light(true);
	m_Primitive[3]->GetMaterial()->SetColor(Color(0.6f, 0.6f, 0.6f));
	// light source 2
	m_Primitive[4] = new Sphere(vector3(2, 5, 1), 0.1f);
	m_Primitive[4]->Light(true);
	m_Primitive[4]->GetMaterial()->SetColor(Color(0.7f, 0.7f, 0.9f));
	*/

	InitialParameter();
	tracer = new Raytracer(n, m, lightnum, thingnum, 19980608);
	tracer->camera = new Camera(viewpoint, n, m, viewplain);
	
	//default initial in phantom
	//param: reflect, diff, spec, refract, rindex
	//m_Color( Color( 0.2f, 0.2f, 0.2f ) ),
	//m_Refl(0), m_Diff(0.2f), m_Spec(0.8f), m_RIndex(1.5f)

	//second scene
	tracer->light[0] = new Pointlight(Material(Color(0.4f, 0.4f, 0.4f), 0, 0, 0, 0, 0), Vec3(0, 5, 5));
	tracer->light[1] = new Pointlight(Material(Color(0.7f, 0.7f, 0.9f), 0, 0, 0, 0, 0), Vec3(-3, 5, 1));
	tracer->thing[0] = new Plain(Vec3(0, 1, 0), 4.4f, Material(Color(0.4f, 0.3f, 0.3f), 0, 1.0f, 0, 0, 0));
	tracer->thing[1] = new Plain(Vec3(0.4f, 0, -1), 12, Material(Color(0.5f, 0.3f, 0.5f), 0, 0.6f, 0, 0, 0));
	tracer->thing[2] = new Plain(Vec3(0, -1, 0), 7.4f, Material(Color(0.4f, 0.7f, 0.7f), 0, 0.5f, 0, 0, 0));
	tracer->thing[3] = new Plain(Vec3(1, 0, 0), 8.0f, Material(Color(0.7f, 0.4f, 0.7f), 0, 0.5f, 0, 0, 0));
	tracer->thing[4] = new Plain(Vec3(-1, 0, 0), 7.0f, Material(Color(0.7f, 0.7f, 0.4f), 0, 0.5f, 0, 0, 0));
	tracer->thing[5] = new Ball(Vec3(2, 0.8f, 7), 2.5f, Material(Color(0.7f, 0.7f, 1.0f), 0.2f, 0.2f, 0.8f, 0.8f, 1.3f));
	tracer->thing[6] = new Ball(Vec3(-5.5f, -0.5f, 7), 2, Material(Color(0.7f, 0.7f, 1.0f), 0.2f, 0.2f, 0.8f, 0.8, 1.3f));
	tracer->thing[7] = new Ball(Vec3(-1.5f, -3.8f, 1), 1.5f, Material(Color(1.0f, 0.4f, 0.4f), 0, 0.2f, 0.8f, 0.8f, 1.5f));
	/*
	int offset = 8;
	for (int x = 0; x < 8; ++x) {
		for (int y = 0; y < 7; ++y) {
			tracer->thing[offset+x*7+y] = new Ball(Vec3(-4.5f+x*1.5f, -4.3f+y*1.5f, 10), 0.3f, Material(Color(0.3f, 1.0f, 0.4f), 0, 0.6f, 0.6f, 0, 1.5f));
		}
	}
	*/
	/*new scene with teapot
	tracer->light[0] = new Pointlight(Material(Color(0.4f, 0.4f, 0.4f), 0, 0, 0, 0, 0), Vec3(0, 5, 5));
	tracer->light[1] = new Pointlight(Material(Color(0.7f, 0.7f, 0.9f), 0, 0, 0, 0, 0), Vec3(-3, 5, 1));
	tracer->thing[0] = new Plain(Vec3(0, 1, 0), 4.4f, Material(Color(0.4f, 0.3f, 0.3f), 0, 1.0f, 0, 0, 0));
	tracer->thing[1] = new Plain(Vec3(0.4f, 0, -1), 12, Material(Color(0.5f, 0.3f, 0.5f), 0, 0.6f, 0, 0, 0));
	for (int i = 0; i < 16; ++i) {
		char number[3];
		sprintf_s(number, "%d", i+4);
		string filename = "bezier/controlvoxel" + string(number) + ".txt";
		std::cout << filename << endl;
		tracer->thing[2+i] = new Bezier(filename, Material(Color(0.4f, 0.7f, 0.7f), 0, 0.6, 0.4, 0, 0));
	}
	*/
	
	/*first scene
	tracer->light[0] = new Pointlight(Material(Color(0.6f, 0.6f, 0.6f), 0, 0, 0, 0, 0), Vec3(0, 5, 5));
	tracer->light[1] = new Pointlight(Material(Color(0.7f, 0.7f, 0.9f), 0, 0, 0, 0, 0), Vec3(2, 5, 1));
	tracer->thing[0] = new Plain(Vec3(0, 1, 0), 4.4f, Material(Color(0.4f, 0.3f, 0.3f), 0, 0.4f, 0, 0, 0));
	tracer->thing[1] = new Ball(Vec3(1, -0.8f, 3), 2.5f, Material(Color(0.7f, 0.7f, 0.7f), 0.6f, 0.2f, 0.8f, 0.8, 1));
	tracer->thing[2] = new Ball(Vec3(-5.5f, -0.5, 5), 2, Material(Color(0.7f, 0.7f, 1.0f), 0.6f, 0.2f, 0.8f, 0.8, 1));
	tracer->thing[3] = new Ball(Vec3(5.0f, -0.8f, 7), 2, Material(Color(0.8f, 0.5f, 0.5f), 1.0f, 0.2f, 0.8f, 0.8, 1));
	tracer->thing[4] = new Plain(Vec3(0, 0, -1), 15, Material(Color(0.4f, 0.3f, 0.3f), 0, 0.4f, 0, 0, 0));
	//tracer->thing[5] = new Plain(Vec3(0, -1, 0), 10, Material(Color(0.4f, 0.3f, 0.3f), 0, 0.4f, 0, 0, 0));
	*/
	tracer->Calculate();
	int count = 0;
	Mat M(m, n, CV_8UC3, Scalar::all(0));
	Mat N(m, n, CV_8UC3, Scalar::all(0));
	//ofstream fout("pixel.txt");
	for (int round = 0; round < 500; ++round) {
		count = 0;
		tracer->ShootPhoton();
		tracer->hplist->CalculatePhotonImage();
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < m; ++j) {
				tracer->photonimage[i][j] = tracer->hplist->photonimage[i][j];
				tracer->photonimage[i][j] /= round + 1;
				drawpixel(i, j, N, tracer->photonimage[i][j] * 255);
				if (!tracer->photonimage[i][j].IsZeroColor())count++;
				tracer->photonimage[i][j] += tracer->image[i][j];
				tracer->photonimage[i][j].Confine();
				tracer->photonimage[i][j] *= 255;
				drawpixel(i, j, M, tracer->photonimage[i][j]);
			}
		}
		char id[4];
		sprintf(id, "%d", round);
		string s = "combinations/" + string(id) + ".png";
		std::cout << s << endl;
		imwrite(s, M);
		s = "images/" + string(id) + ".png";
		std::cout << s << endl;
		imwrite(s, N);
		printf("%d\n", count);
	}
	cvDestroyAllWindows();
	return 0;
}

void InitialParameter() {
	viewpoint = Vec3::Vec3(0, 0, -5);//相机视点
	viewplain = new Vec3*[n];
	for (int i = 0; i < n; ++i) {
		viewplain[i] = new Vec3[m];
		for (int j = 0; j < m; ++j) {
			viewplain[i][j] = Vec3::Vec3(-4 + 0.008*i, 4-0.008*j, 0);
		}
	}
}


void InitialMatrix() {
	
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			standard_matrix[i][j].x = rand() % 1000;
			standard_matrix[i][j].y = rand() % 1000;
			standard_matrix[i][j].z = rand() % 1000;
		}
	}
	
	/*
	standard_matrix[0][0].x = 100;
	standard_matrix[0][0].y = 400;
	standard_matrix[0][1].x = 200;
	standard_matrix[0][1].y = 300;
	standard_matrix[0][2].x = 300;
	standard_matrix[0][2].y = 320;
	standard_matrix[0][3].x = 400;
	standard_matrix[0][3].y = 400;
	standard_matrix[1][0].x = 110;
	standard_matrix[1][0].y = 300;
	standard_matrix[1][1].x = 210;
	standard_matrix[1][1].y = 280;
	standard_matrix[1][2].x = 300;
	standard_matrix[1][2].y = 280;
	standard_matrix[1][3].x = 380;
	standard_matrix[1][3].y = 320;
	standard_matrix[2][0].x = 130;
	standard_matrix[2][0].y = 250;
	standard_matrix[2][1].x = 250;
	standard_matrix[2][1].y = 200;
	standard_matrix[2][2].x = 330;
	standard_matrix[2][2].y = 180;
	standard_matrix[2][3].x = 410;
	standard_matrix[2][3].y = 200;
	standard_matrix[3][0].x = 150;
	standard_matrix[3][0].y = 350;
	standard_matrix[3][1].x = 280;
	standard_matrix[3][1].y = 250;
	standard_matrix[3][2].x = 360;
	standard_matrix[3][2].y = 250;
	standard_matrix[3][3].x = 480;
	standard_matrix[3][3].y = 280;
	*/
}