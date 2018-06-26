#pragma once
#include <cv.h>
#include <highgui.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>
#include <cstdio>
#include <cmath>
#include "color.h"

using namespace cv;
using namespace std;

void drawpixel(int x, int y, Mat &m, Color c);
void IntegerBresenhamline(int x0, int y0, int x1, int y1, Mat &m);
void linex(int x0, int y0, int x1, int y1, Mat &m);
void liney(int x0, int y0, int x1, int y1, Mat &m);
void swap(int &a, int &b);

void swap(int &a, int &b) {
	int c = a;
	a = b;
	b = c;
}

void drawpixel(int x, int y, Mat &m, Color c = Color::Color(0, 0, 0)) {//rgb
	m.at<Vec3b>(y, x) = Vec3b(c.b, c.g, c.r);
}

void IntegerBresenhamline(int x0, int y0, int x1, int y1, Mat &m) {
	int dx, dy;
	dx = x1 - x0, dy = y1 - y0;
	if (abs(dx) > abs(dy))
		linex(x0, y0, x1, y1, m);
	else
		liney(x0, y0, x1, y1, m);
}

void linex(int x0, int y0, int x1, int y1, Mat &m) {
	int x, y, dx, dy, e;
	if (x0 > x1) {
		swap(x0, x1);
		swap(y0, y1);
	}
	if (y1 - y0 >= 0) {
		dx = x1 - x0, dy = y1 - y0, e = -dx;
		x = x0, y = y0;
		for (int i = 0; i <= dx; ++i) {
			drawpixel(x, y, m);
			x++, e = e + 2 * dy;
			if (e>0) {
				y++; e = e - 2 * dx;
			}
		}
	}
	else {
		dx = x1 - x0, dy = y1 - y0, e = dx;
		x = x0, y = y0;
		for (int i = 0; i <= dx; ++i) {
			drawpixel(x, y, m);
			x++, e = e + 2 * dy;
			if (e<0) {
				y--; e = e + 2 * dx;
			}
		}
	}

}

void liney(int x0, int y0, int x1, int y1, Mat &m) {
	int x, y, dx, dy, e;
	if (y0 > y1) {
		swap(x0, x1);
		swap(y0, y1);
	}
	if (x1 - x0 >= 0) {
		dx = x1 - x0, dy = y1 - y0, e = -dx;
		x = x0, y = y0;
		for (int i = 0; i <= dy; ++i) {
			drawpixel(x, y, m);
			y++, e = e + 2 * dx;
			if (e>0) {
				x++; e = e - 2 * dy;
			}
		}
	}
	else {
		dx = x1 - x0, dy = y1 - y0, e = dx;
		x = x0, y = y0;
		for (int i = 0; i <= dy; ++i) {
			drawpixel(x, y, m);
			y++, e = e + 2 * dx;
			if (e<0) {
				x--; e = e + 2 * dy;
			}
		}
	}
}