#include <array>
#include <iostream>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <functional>
#include <SDL.h>
#include <vector>

constexpr int SCREEN_WIDTH = 640;
constexpr int SCREEN_HEIGHT = 480;

void draw_line_zingl(SDL_Renderer* r, const SDL_Point a, const SDL_Point b, const SDL_Color c)
{
	const int dx = std::abs(b.x - a.x);
	const int sx = a.x < b.x ? 1 : -1;
	const int dy = -std::abs(b.y - a.y);
	const int sy = a.y < b.y ? 1 : - 1;
	
	int x = a.x;
	int y = a.y;
	int err = dx + dy;
	
	SDL_SetRenderDrawColor(r, c.r, c.g, c.b, c.a);
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


void draw_line_bresenham(SDL_Renderer* r, const SDL_Point a, const SDL_Point b, const SDL_Color c)
{
	const int dx = std::abs(b.x - a.x);
	const int dy = std::abs(b.y - a.y);
	int x;
	int y;
	SDL_SetRenderDrawColor(r, c.r, c.g, c.b, c.a);
	
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


void draw_line_dda(SDL_Renderer* r, const SDL_Point a, const SDL_Point b, const SDL_Color c)
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

	SDL_SetRenderDrawColor(r, c.r, c.g, c.b, c.a);
	SDL_RenderDrawPoint(r, std::round(x), std::round(y));
	for (int k = 0; k < steps; k++)
	{
		x += x_incr;
		y += y_incr;
		SDL_RenderDrawPoint(r, std::round(x), std::round(y));
	}
}


// i = independent variable
// d = dependent variable
std::vector<int> interpolate(const int i0, const int d0, const int i1, const int d1)
{
	const int num_vals = i1 - i0 + 1;
	std::vector<int> dep_vals(num_vals);
	const double slope = static_cast<double>(d1 - d0) / (i1 - i0);
	double d = static_cast<double>(d0);
	for (std::size_t i = 0; i < dep_vals.size(); i++)
	{
		dep_vals.at(i) = std::round(d);
		d += slope;
	}
	return dep_vals;
}


void draw_line_interpolation(SDL_Renderer* r, SDL_Point p0, SDL_Point p1, const SDL_Color c)
{
	const int dx = p1.x - p0.x;
	const int dy = p1.y - p0.y;
	SDL_SetRenderDrawColor(r, c.r, c.g, c.b, c.a);
	if (std::abs(dx) > std::abs(dy))
	{
		// The line is longer horizontally
		if (p0.x > p1.x)
			std::swap(p0, p1);
		std::vector<int> ys = interpolate(p0.x, p0.y, p1.x, p1.y);
		// Draw all the pixels in the line
		SDL_SetRenderDrawColor(r, c.r, c.g, c.b, c.a);
		const int x0 = p0.x;
		for (int x = x0; x <= p1.x; x++)
		{
			int y = ys.at(x - x0);
			SDL_RenderDrawPoint(r, x, y);
		}
	}
	else
	{
		// The line is longer vertically
		if (p0.y > p1.y)
			std::swap(p0, p1);
		std::vector<int> xs = interpolate(p0.y, p0.x, p1.y, p1.x);
		// Draw all the pixels in the line
		const int y0 = p0.y;
		for (int y = y0; y <= p1.y; y++)
		{
			int x = xs.at(y - y0);
			SDL_RenderDrawPoint(r, x, y);
		}
	}
}


int main()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cerr << "SDL_Init error: " << SDL_GetError() << "\n";
		return EXIT_FAILURE;
	}

	SDL_Window* window = SDL_CreateWindow(
		"2D Drawing Algorithms",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN
	);
	if (window == nullptr)
	{
		std::cerr << "Window error: " << SDL_GetError() << "\n";
		return EXIT_FAILURE;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer == nullptr)
	{
		std::cerr << "Renderer error: " << SDL_GetError() << "\n";
		return EXIT_FAILURE;
	}

	SDL_Color red   = {255,   0,   0,   0};
	SDL_Color green = {  0, 255,   0,   0};
	SDL_Color blue  = {  0,   0, 255,   0};
	SDL_Color black = {  0,   0,   0,   0};

	const std::array<std::function<void(SDL_Renderer*, SDL_Point, SDL_Point, SDL_Color)>, 4> drawing_funcs = {
		&draw_line_interpolation,
		&draw_line_dda,
		&draw_line_bresenham,
		&draw_line_zingl
	};
	const std::array<std::string, 4> func_names = {
		"Interpolation",
		"DDA",
		"Bresenham",
		"Zingl"
	};
	const std::array<SDL_Color, 4> line_colors = {{
		red,
		green,
		blue,
		black
	}};
	
	const int screen_mid = SCREEN_HEIGHT / 2;
	const std::array<SDL_Point, 6> start_points = {{
		{100, SCREEN_HEIGHT - 1},
		{100, SCREEN_HEIGHT - 1},
		{0, screen_mid},
		{0, screen_mid},
		{100, 0},
		{100, 0}
	}};
	const std::array<SDL_Point, 6> end_points = {{
		{100, 0},
		{150, 0},
		{SCREEN_WIDTH - 1, screen_mid - 100},
		{SCREEN_WIDTH - 1, screen_mid + 100},
		{150, SCREEN_HEIGHT - 1},
		{100, SCREEN_HEIGHT - 1}
	}};
	
	bool should_exit_early = false;
	SDL_Event event;
	for (std::size_t i = 0; i < drawing_funcs.size() && !should_exit_early; i++)
	{
		auto func = drawing_funcs.at(i);
		const std::string func_name = func_names.at(i);
		const SDL_Color line_color = line_colors.at(i);
		std::cout << i + 1 << ". " << func_name << std::endl;
		
		for (std::size_t j = 0; j < start_points.size(); j++)
		{
			const SDL_Point start_point = start_points.at(j);
			const SDL_Point end_point = end_points.at(j);
			
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			SDL_RenderClear(renderer);
			
			const auto time_start = std::chrono::system_clock::now();
			func(renderer, start_point, end_point, line_color);
			const auto time_end = std::chrono::system_clock::now();
			const auto us_elapsed = std::chrono::duration_cast<std::chrono::microseconds>(time_end - time_start);
			
			SDL_RenderPresent(renderer);
			std::cout << "(" << start_point.x << ", " << start_point.y << ") to ";
			std::cout << "(" << end_point.x << ", " << end_point.y << "): ";
			std::cout << us_elapsed.count() << " us\n";
			
			bool should_continue = false;
			do
			{
				SDL_PollEvent(&event);
				if (event.type == SDL_QUIT)
				{
					should_continue = true;
					should_exit_early = true;	
					
				}
				else if (event.type == SDL_KEYDOWN)
				{
					switch (event.key.keysym.sym)
					{
						case SDLK_RETURN:
						case SDLK_SPACE:
							should_continue = true;
							break;
						case SDLK_ESCAPE:
							should_continue = true;
							should_exit_early = true;
							break;
						default:
							break;
					}
				}
			}
			while (!should_continue);
		}
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
	return EXIT_SUCCESS;
}
