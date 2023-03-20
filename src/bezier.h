#ifndef BEZIER_H
#define BEZIER_H

#include <tuple>
#include <vector>

double bezier_quad(const double t, const int c0, const int c1, const int c2);
double bezier_quad_d1(const double t, const int c0, const int c1, const int c2);
double bezier_quad_d2(const int c0, const int c1, const int c2);
std::tuple<int, int> get_bezier_quad_minmax(const int c0, const int c1, const int c2);

double bezier_cubic(const double t, const int c0, const int c1, const int c2, const int c3);
double bezier_cubic_d1(const double t, const int c0, const int c1, const int c2, const int c3);
double bezier_cubic_d2(const double t, const int c0, const int c1, const int c2, const int c3);
std::tuple<int, int> get_bezier_cubic_minmax(const int c0, const int c1, const int c2, const int c3);

// The following algorithms were all taken from:
// "A Rasterizing Algorithm for Drawing Curves" by Alois Zingl
void draw_bezier_quad_seg(std::vector<std::uint32_t>& pixels,
	const unsigned int width, const std::uint32_t color,
	int x0, int y0, int x1, int y1, int x2, int y2);
void draw_bezier_quad(std::vector<std::uint32_t>& pixels,
	const unsigned int width, const std::uint32_t color,
	int x0, int y0, int x1, int y1, int x2, int y2);
void draw_bezier_cubic_seg(std::vector<std::uint32_t>& pixels,
	const unsigned int width, const std::uint32_t color,
	int x0, int y0, float x1, float y1, float x2, float y2, int x3, int y3);
void draw_bezier_cubic(std::vector<std::uint32_t>& pixels,
	const unsigned int width, const std::uint32_t color,
	int x0, int y0, float x1, float y1, float x2, float y2, int x3, int y3);

#endif
