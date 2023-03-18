#ifndef BEZIER_H
#define BEZIER_H

#include <SDL.h>

// The following algorithms were all taken from:
// "A Rasterizing Algorithm for Drawing Curves" by Alois Zingl
void draw_bezier_quad_seg(SDL_Renderer* r, int x0, int y0, int x1, int y1, int x2, int y2);
void draw_bezier_quad(SDL_Renderer* r, int x0, int y0, int x1, int y1, int x2, int y2);
void draw_bezier_cubic_seg(SDL_Renderer* r, int x0, int y0, float x1, float y1, float x2, float y2, int x3, int y3);
void draw_bezier_cubic(SDL_Renderer* r, int x0, int y0, float x1, float y1, float x2, float y2, int x3, int y3);

#endif