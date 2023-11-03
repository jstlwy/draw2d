#include <array>
#include <cstdint>
#include <iostream>
#include <chrono>
#include <functional>
#include <vector>
#include "line.h"
#include "circle.h"
#include "fill.h"
#include "svg.h"
#include "graphics.h"
#include "constants.h"
#include <SDL2/SDL.h>

void run_tests();
bool wait_for_input();

int main()
{
    try {
        run_tests();
    } catch (std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}

void run_tests()
{
    Graphics gfx;

    static const std::array<std::function<void(std::vector<std::uint32_t>&, const std::uint32_t, const int, const int, const int, const int)>, 3> drawing_funcs = {
        &draw_line_dda,
        &draw_line_bresenham,
        &draw_line_zingl
    };
    static const std::array<std::string, 3> func_names = {
        "DDA",
        "Bresenham",
        "Zingl"
    };
    static const std::array<std::uint32_t, 3> line_colors = {{
        red,
        green,
        blue
    }};

    static const std::array<SDL_Point, 7> line_pas = {{
        {100, SCREEN_HEIGHT - 1},
        {100, SCREEN_HEIGHT - 1},
        {  0, Y_MID_SCREEN},
        {  0, Y_MID_SCREEN},
        {  0, Y_MID_SCREEN},
        {100, 0},
        {100, 0}
    }};
    static const std::array<SDL_Point, 7> line_pbs = {{
        {100, 0},
        {150, 0},
        {SCREEN_WIDTH - 1, Y_MID_SCREEN - 100},
        {SCREEN_WIDTH - 1, Y_MID_SCREEN},
        {SCREEN_WIDTH - 1, Y_MID_SCREEN + 100},
        {150, SCREEN_HEIGHT - 1},
        {100, SCREEN_HEIGHT - 1}
    }};

    std::cout << "LINE DRAWING FUNCTIONS\n\n";
    bool should_exit_early = false;
    for (std::size_t i = 0; i < drawing_funcs.size() && !should_exit_early; i++) {
        const std::string func_name = func_names.at(i);
        std::cout << i + 1 << ". " << func_name << std::endl;

        auto func = drawing_funcs.at(i);
        const std::uint32_t line_color = line_colors.at(i);

        for (std::size_t j = 0; j < line_pas.size(); j++) {
            const SDL_Point pa = line_pas.at(j);
            const SDL_Point pb = line_pbs.at(j);

            const auto time_start = std::chrono::system_clock::now();
            func(gfx.pixels, line_color, pa.x, pa.y, pb.x, pb.y);
            const auto time_end = std::chrono::system_clock::now();
            const auto us_elapsed = std::chrono::duration_cast<std::chrono::microseconds>(time_end - time_start);
            std::cout << "(" << pa.x << ", " << pa.y << ") to ";
            std::cout << "(" << pb.x << ", " << pb.y << "): ";
            std::cout << us_elapsed.count() << " us\n";

            gfx.render();
            should_exit_early = wait_for_input();
            if (should_exit_early) {
                break;
            }
        }
        std::cout << "\n";
    }
    if (should_exit_early) {
        return;
    }

    std::cout << "CIRCLE DRAWING FUNCTION\n\n";
    const SDL_Point circle_center = {X_MID_SCREEN, Y_MID_SCREEN};
    int radius = SCREEN_HEIGHT / 4;
    draw_circle_midpoint(gfx.pixels, black, circle_center.x, circle_center.y, radius);
    flood_fill_stack(gfx.pixels, black, circle_center.x, circle_center.y);
    gfx.render();
    if (wait_for_input()) {
        return;
    }

    std::cout << "SVG DRAWING FUNCTION\n\n";
    draw_svg(gfx.pixels, black, "19976.svg");
    gfx.render();
    wait_for_input();
}

bool wait_for_input()
{
    bool should_exit_early = false;
    bool should_continue = false;
    SDL_Event event;
    do {
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) {
            should_continue = true;
            should_exit_early = true;	
        } else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
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
    } while (!should_continue);
    return should_exit_early;
}
