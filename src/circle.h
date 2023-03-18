#ifndef CIRCLE_H
#define CIRCLE_H

#include <vector>

// cx, cy = circle center coordinates
void plot_circle_points(std::vector<std::uint32_t>& pixels, const unsigned int rowlen,
    const std::uint32_t color, const int cx, const int cy, const int x, const int y);
void draw_circle_midpoint(std::vector<std::uint32_t>& pixels, const unsigned int rowlen,
    const std::uint32_t color, const int cx, const int cy, const int radius);

#endif