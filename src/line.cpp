#include "line.h"
#include <cmath>

void draw_line_zingl(std::vector<std::uint32_t>& pixels, const unsigned int width, 
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
		pixels.at((y * width) + x) = color;
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


void draw_line_bresenham(std::vector<std::uint32_t>& pixels, const unsigned int width,
	const std::uint32_t color, const int ax, const int ay, const int bx, const int by)
{
	const int dx = std::abs(bx - ax);
	const int dy = std::abs(by - ay);
	int x = ax;
	int y = ay;

	if (dx == 0)
	{
		int y_end;
		if (ay < by)
		{
			y_end = by;
		}
		else
		{
			y = by;
			y_end = ay;
		}
		int row = y * width;
		const int row_end = y_end * width;
		while (row <= row_end)
		{
			pixels.at(row + x) = color;
			row += width;
		}
	}
	else if (dy == 0)
	{
		int x_end;
		if (ax < bx)
		{
			x_end = bx;
		}
		else
		{
			x = bx;
			x_end = ax;
		}
		const int row = y * width;
		while (x <= x_end)
		{
			pixels.at(row + x) = color;
			x++;
		}
	}
	else if (dx > dy)
	{
		//int sy = 1;
		int srow = width;
		int x_end;
		if (ax < bx)
		{
			x_end = bx;
			if (y > by)
				srow = -width;
		}
		else
		{
			x = bx;
			x_end = ax;
			y = by;
			if (y > ay)
				srow = -width;
		}

		const int two_dy = 2 * dy;
		const int two_diff_dy_dx = 2 * (dy - dx);
		int p = two_dy - dx;
		int row = y * width;
		
		while (x <= x_end)
		{
			pixels.at(row + x) = color;
			x++;
			if (p < 0)
			{
				p += two_dy;
			}
			else
			{
				//y += sy;
				row += srow;
				p += two_diff_dy_dx;
			}
		}
	}
	else
	{
		int sx = 1;
		int y_end;
		if (ay < by)
		{
			if (x > bx)
				sx = -1;
			y_end = by;
		}
		else
		{
			x = bx;
			if (x > ax)
				sx = -1;
			y = by;
			y_end = ay;
		}

		const int two_dx = 2 * dx;
		const int two_diff_dx_dy = 2 * (dx - dy);
		int p = two_dx - dy;
		int row = y * width;
		const int row_end = y_end * width;
		
		//while (y <= y_end)
		while (row <= row_end)
		{
			pixels.at(row + x) = color;
			//y++;
			row += width;
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


void draw_line_dda(std::vector<std::uint32_t>& pixels, const unsigned int width,
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

	pixels.at((std::round(y) * width) + std::round(x)) = color;
	for (int k = 0; k < steps; k++)
	{
		x += x_incr;
		y += y_incr;
		pixels.at((std::round(y) * width) + std::round(x)) = color;
	}
}
