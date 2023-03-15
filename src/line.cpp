#include "line.h"
#include <cmath>

void draw_line_zingl(SDL_Renderer* r, const SDL_Point a, const SDL_Point b)
{
	const int dx = std::abs(b.x - a.x);
	const int sx = a.x < b.x ? 1 : -1;
	const int dy = -std::abs(b.y - a.y);
	const int sy = a.y < b.y ? 1 : - 1;
	
	int x = a.x;
	int y = a.y;
	int err = dx + dy;
	
	while (true)
	{
		SDL_RenderDrawPoint(r, x, y);
		const int e2 = 2 * err;
		if (e2 >= dy)
		{
			if (x == b.x)
				break;
			err += dy;
			x += sx;
		}
		if (e2 <= dx)
		{
			if (y == b.y)
				break;
			err += dx;
			y += sy;
		}
	}
}


void draw_line_bresenham(SDL_Renderer* r, const SDL_Point a, const SDL_Point b)
{
	const int dx = std::abs(b.x - a.x);
	const int dy = std::abs(b.y - a.y);
	int x;
	int y;
	
	if (dx > dy)
	{
		int sy = 1;
		int x_end;
		if (a.x > b.x)
		{
			x = b.x;
			x_end = a.x;
			y = b.y;
			if (y > a.y)
				sy = -1;
		}
		else
		{
			x = a.x;
			x_end = b.x;
			y = a.y;
			if (y > b.y)
				sy = -1;
		}

		const int two_dy = 2 * dy;
		const int two_diff_dy_dx = 2 * (dy - dx);
		int p = two_dy - dx;
		
		while (x <= x_end)
		{
			SDL_RenderDrawPoint(r, x, y);
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
		if (a.y > b.y)
		{
			x = b.x;
			if (x > a.x)
				sx = -1;
			y = b.y;
			y_end = a.y;
		}
		else
		{
			x = a.x;
			if (x > b.x)
				sx = -1;
			y = a.y;
			y_end = b.y;
		}

		const int two_dx = 2 * dx;
		const int two_diff_dx_dy = 2 * (dx - dy);
		int p = two_dx - dy;
		
		while (y <= y_end)
		{
			SDL_RenderDrawPoint(r, x, y);
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


void draw_line_dda(SDL_Renderer* r, const SDL_Point a, const SDL_Point b)
{
	const int dx = b.x - a.x;
	const int dy = b.y - a.y;

	const int abs_dx = std::abs(dx);
	const int abs_dy = std::abs(dy);
	const int steps = abs_dx > abs_dy ? abs_dx : abs_dy;

	double x = static_cast<double>(a.x);
	double y = static_cast<double>(a.y);
	const double x_incr = dx / static_cast<double>(steps);
	const double y_incr = dy / static_cast<double>(steps);

	SDL_RenderDrawPoint(r, std::round(x), std::round(y));
	for (int k = 0; k < steps; k++)
	{
		x += x_incr;
		y += y_incr;
		SDL_RenderDrawPoint(r, std::round(x), std::round(y));
	}
}
