#include <array>
#include <iostream>
#include <chrono>
#include <functional>
#include <vector>
#include <SDL.h>
#include "line.h"
#include "circle.h"
#include "fill.h"
#include "svg.h"

constexpr unsigned int SCREEN_WIDTH = 1920;
constexpr unsigned int SCREEN_HEIGHT = 1080;
constexpr unsigned int X_MID_SCREEN = SCREEN_WIDTH / 2;
constexpr unsigned int Y_MID_SCREEN = SCREEN_HEIGHT / 2;
constexpr unsigned int NUM_PIXELS = SCREEN_WIDTH * SCREEN_HEIGHT;
constexpr unsigned int TEXTURE_PITCH = SCREEN_WIDTH * sizeof(std::uint32_t);

void render_texture(SDL_Renderer* renderer, SDL_Texture* texture, const std::vector<std::uint32_t>& pixels);
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
		std::cerr << "Error when creating renderer: " << SDL_GetError() << std::endl;
		return EXIT_FAILURE;
	}

	SDL_Texture* texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STATIC,
		SCREEN_WIDTH,
		SCREEN_HEIGHT
	);
	if (texture == nullptr)
	{
		std::cerr << "Error when creating texture: " << SDL_GetError() << std::endl;
		return EXIT_FAILURE;
	}

	const std::uint32_t red   = 0xFFFF0000;
	const std::uint32_t green = 0xFF00FF00;
	const std::uint32_t blue  = 0xFF0000FF;
	const std::uint32_t black = 0xFF000000;
	const std::uint32_t blank = 0x00FFFFFF;

	std::vector<std::uint32_t> pixels(NUM_PIXELS);

	const std::array<std::function<void(std::vector<std::uint32_t>&, const std::uint32_t,
		const unsigned int, const int, const int, const int, const int)>, 3> drawing_funcs = {
		&draw_line_dda,
		&draw_line_bresenham,
		&draw_line_zingl
	};
	const std::array<std::string, 3> func_names = {
		"DDA",
		"Bresenham",
		"Zingl"
	};
	const std::array<std::uint32_t, 3> line_colors = {{
		red,
		green,
		blue
	}};

	const std::array<SDL_Point, 7> line_pas = {{
		{100, SCREEN_HEIGHT - 1},
		{100, SCREEN_HEIGHT - 1},
		{0, Y_MID_SCREEN},
		{0, Y_MID_SCREEN},
		{0, Y_MID_SCREEN},
		{100, 0},
		{100, 0}
	}};
	const std::array<SDL_Point, 7> line_pbs = {{
		{100, 0},
		{150, 0},
		{SCREEN_WIDTH - 1, Y_MID_SCREEN - 100},
		{SCREEN_WIDTH - 1, Y_MID_SCREEN},
		{SCREEN_WIDTH - 1, Y_MID_SCREEN + 100},
		{150, SCREEN_HEIGHT - 1},
		{100, SCREEN_HEIGHT - 1}
	}};

	// LINE DRAWING TEST
	bool should_exit_early = false;
	for (std::size_t i = 0; i < drawing_funcs.size() && !should_exit_early; i++)
	{
		const std::string func_name = func_names.at(i);
		std::cout << i + 1 << ". " << func_name << std::endl;

		auto func = drawing_funcs.at(i);
		const std::uint32_t line_color = line_colors.at(i);

		for (std::size_t j = 0; j < line_pas.size(); j++)
		{
			std::fill(pixels.begin(), pixels.end(), blank);
			const SDL_Point pa = line_pas.at(j);
			const SDL_Point pb = line_pbs.at(j);

			const auto time_start = std::chrono::system_clock::now();
			func(pixels, SCREEN_WIDTH, line_color, pa.x, pa.y, pb.x, pb.y);
			const auto time_end = std::chrono::system_clock::now();
			const auto us_elapsed = std::chrono::duration_cast<std::chrono::microseconds>(time_end - time_start);
			std::cout << "(" << pa.x << ", " << pa.y << ") to ";
			std::cout << "(" << pb.x << ", " << pb.y << "): ";
			std::cout << us_elapsed.count() << " us" << std::endl;

			render_texture(renderer, texture, pixels);
			should_exit_early = wait_for_input();
		}
	}

	// CIRCLE DRAWING TEST
	std::fill(pixels.begin(), pixels.end(), blank);
	const SDL_Point circle_center = {X_MID_SCREEN, Y_MID_SCREEN};
	int radius = SCREEN_HEIGHT / 4;
	draw_circle_midpoint(pixels, SCREEN_WIDTH, black, circle_center.x, circle_center.y, radius);
	flood_fill(pixels, SCREEN_WIDTH, SCREEN_HEIGHT, black, circle_center.x, circle_center.y);
	render_texture(renderer, texture, pixels);
	should_exit_early = wait_for_input();

	// SVG DRAWING TEST
	std::fill(pixels.begin(), pixels.end(), blank);
	draw_svg(pixels, SCREEN_WIDTH, SCREEN_HEIGHT, black, "19976.svg");
	render_texture(renderer, texture, pixels);
	should_exit_early = wait_for_input();

	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return EXIT_SUCCESS;
}

void render_texture(SDL_Renderer* renderer, SDL_Texture* texture, const std::vector<std::uint32_t>& pixels)
{
	SDL_RenderClear(renderer);
	SDL_UpdateTexture(texture, nullptr, &pixels[0], TEXTURE_PITCH);
	SDL_RenderCopy(renderer, texture, nullptr, nullptr);
	SDL_RenderPresent(renderer);
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
