#include "circle.h"

void plot_circle_points(SDL_Renderer* r, const SDL_Point center, const int x, const int y)
{
	SDL_RenderDrawPoint(r, center.x + x, center.y + y);
	SDL_RenderDrawPoint(r, center.x - x, center.y + y);
	SDL_RenderDrawPoint(r, center.x + x, center.y - y);
	SDL_RenderDrawPoint(r, center.x - x, center.y - y);
	SDL_RenderDrawPoint(r, center.x + y, center.y + x);
	SDL_RenderDrawPoint(r, center.x - y, center.y + x);
	SDL_RenderDrawPoint(r, center.x + y, center.y - x);
	SDL_RenderDrawPoint(r, center.x - y, center.y - x);
}

void draw_circle_midpoint(SDL_Renderer* r, const SDL_Point center, const int radius)
{
	int x = 0;
	int y = radius;
	int p = 1 - radius;
	
	plot_circle_points(r, center, x, y);
	
	while (x < y)
	{
		x++;
		if (p < 0)
		{
			p += (2 * x) + 1;
		}
		else
		{
			y--;
			p += (2 * (x - y)) + 1;	
		}
		plot_circle_points(r, center, x, y);	
	}
}
