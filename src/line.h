#ifndef LINE_H
#define LINE_H

#include <SDL.h>

void draw_line_zingl(SDL_Renderer* r, const SDL_Point a, const SDL_Point b);
void draw_line_bresenham(SDL_Renderer* r, const SDL_Point a, const SDL_Point b);
void draw_line_dda(SDL_Renderer* r, const SDL_Point a, const SDL_Point b);

#endif