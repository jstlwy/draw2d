#include <array>
#include <iostream>
#include <chrono>
#include <functional>
#include <SDL.h>
#include "line.h"
#include "circle.h"

constexpr int SCREEN_WIDTH = 1920;
constexpr int SCREEN_HEIGHT = 1080;

bool wait_for_input();

int main()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cerr << "SDL_Init error: " << SDL_GetError() << std::endl;
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
		std::cerr << "Window error: " << SDL_GetError() << std::endl;
		return EXIT_FAILURE;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer == nullptr)
	{
		std::cerr << "Renderer error: " << SDL_GetError() << std::endl;
		return EXIT_FAILURE;
	}

	SDL_Color red   = {255,   0,   0,   0};
	SDL_Color green = {  0, 255,   0,   0};
	SDL_Color blue  = {  0,   0, 255,   0};
	SDL_Color black = {  0,   0,   0,   0};

	const std::array<std::function<void(SDL_Renderer*, SDL_Point, SDL_Point)>, 3> drawing_funcs = {
		&draw_line_dda,
		&draw_line_bresenham,
		&draw_line_zingl
	};
	const std::array<std::string, 3> func_names = {
		"DDA",
		"Bresenham",
		"Zingl"
	};
	const std::array<SDL_Color, 3> line_colors = {{
		red,
		green,
		blue
	}};
	
	const int screen_x_mid = SCREEN_WIDTH / 2;
	const int screen_y_mid = SCREEN_HEIGHT / 2;
	const std::array<SDL_Point, 6> start_points = {{
		{100, SCREEN_HEIGHT - 1},
		{100, SCREEN_HEIGHT - 1},
		{0, screen_y_mid},
		{0, screen_y_mid},
		{100, 0},
		{100, 0}
	}};
	const std::array<SDL_Point, 6> end_points = {{
		{100, 0},
		{150, 0},
		{SCREEN_WIDTH - 1, screen_y_mid - 100},
		{SCREEN_WIDTH - 1, screen_y_mid + 100},
		{150, SCREEN_HEIGHT - 1},
		{100, SCREEN_HEIGHT - 1}
	}};
	
	bool should_exit_early = false;
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
			SDL_SetRenderDrawColor(renderer, line_color.r, line_color.g, line_color.b, line_color.a);
			
			const auto time_start = std::chrono::system_clock::now();
			func(renderer, start_point, end_point);
			const auto time_end = std::chrono::system_clock::now();
			const auto us_elapsed = std::chrono::duration_cast<std::chrono::microseconds>(time_end - time_start);
			
			SDL_RenderPresent(renderer);
			std::cout << "(" << start_point.x << ", " << start_point.y << ") to ";
			std::cout << "(" << end_point.x << ", " << end_point.y << "): ";
			std::cout << us_elapsed.count() << " us" << std::endl;
			
			should_exit_early = wait_for_input();
		}
	}
	
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a);
	SDL_Point circle_center = {screen_x_mid, screen_y_mid};
	int radius = SCREEN_HEIGHT / 4;
	draw_circle_midpoint(renderer, circle_center, radius);
	SDL_RenderPresent(renderer);
	should_exit_early = wait_for_input();
	
	SDL_DestroyWindow(window);
	SDL_Quit();
	return EXIT_SUCCESS;
}

bool wait_for_input()
{
	bool should_exit_early = false;
	bool should_continue = false;
	SDL_Event event;
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
	return should_exit_early;
}