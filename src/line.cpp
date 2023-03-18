#include "line.h"
#include <cmath>

void draw_line_zingl(std::vector<std::uint32_t>& pixels, const unsigned int rowlen, 
	const std::uint32_t color, const int ax, const int ay, const int bx, const int by)
{
	const int dx = std::abs(bx - ax);
	const int sx = ax < bx ? 1 : -1;
	const int dy = -std::abs(by - ay);
	const int sy = ay < by ? 1 : - 1;
	
	int x = ax;
	int y = ay;
	int err = dx + dy;
	
	while (true)
	{
		pixels.at((y * rowlen) + x) = color;
		const int e2 = 2 * err;
		if (e2 >= dy)
		{
			if (x == bx)
				break;
			err += dy;
			x += sx;
		}
		if (e2 <= dx)
		{
			if (y == by)
				break;
			err += dx;
			y += sy;
		}
	}
}


void draw_line_bresenham(std::vector<std::uint32_t>& pixels, const unsigned int rowlen,
	const std::uint32_t color, const int ax, const int ay, const int bx, const int by)
{
	const int dx = std::abs(bx - ax);
	const int dy = std::abs(by - ay);
	int x;
	int y;
	
	if (dx > dy)
	{
		int sy = 1;
		int x_end;
		if (ax > bx)
		{
			x = bx;
			x_end = ax;
			y = by;
			if (y > ay)
				sy = -1;
		}
		else
		{
			x = ax;
			x_end = bx;
			y = ay;
			if (y > by)
				sy = -1;
		}

		const int two_dy = 2 * dy;
		const int two_diff_dy_dx = 2 * (dy - dx);
		int p = two_dy - dx;
		
		while (x <= x_end)
		{
			pixels.at((y * rowlen) + x) = color;
			x++;
			if (p < 0)
			{
				p += two_dy;
			}
			else
			{
				y += sy;
				p += two_diff_dy_dx;
			}
		}
	}
	else
	{
		int sx = 1;
		int y_end;
		if (ay > by)
		{
			x = bx;
			if (x > ax)
				sx = -1;
			y = by;
			y_end = ay;
		}
		else
		{
			x = ax;
			if (x > bx)
				sx = -1;
			y = ay;
			y_end = by;
		}

		const int two_dx = 2 * dx;
		const int two_diff_dx_dy = 2 * (dx - dy);
		int p = two_dx - dy;
		
		while (y <= y_end)
		{
			pixels.at((y * rowlen) + x) = color;
			y++;
			if (p < 0)
			{
				p += two_dx;
			}
			else
			{
				x += sx;
				p += two_diff_dx_dy;
			}
		}
	}
}


void draw_line_dda(std::vector<std::uint32_t>& pixels, const unsigned int rowlen,
	const std::uint32_t color, const int ax, const int ay, const int bx, const int by)
{
	const int dx = bx - ax;
	const int dy = by - ay;

	const int abs_dx = std::abs(dx);
	const int abs_dy = std::abs(dy);
	const int steps = abs_dx > abs_dy ? abs_dx : abs_dy;

	double x = static_cast<double>(ax);
	double y = static_cast<double>(ay);
	const double x_incr = dx / static_cast<double>(steps);
	const double y_incr = dy / static_cast<double>(steps);

	pixels.at((std::round(y) * rowlen) + std::round(x)) = color;
	for (int k = 0; k < steps; k++)
	{
		x += x_incr;
		y += y_incr;
		pixels.at((std::round(y) * rowlen) + std::round(x)) = color;
	}
}
