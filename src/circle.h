#ifndef CIRCLE_H
#define CIRCLE_H

#include <SDL.h>

void plot_circle_points(SDL_Renderer* r, const SDL_Point center, const int x, const int y);
void draw_circle_midpoint(SDL_Renderer* r, const SDL_Point center, const int radius);

#endif