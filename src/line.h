#ifndef LINE_H
#define LINE_H

#include <vector>
#include <cstdint>

void draw_line_zingl(
    std::vector<std::uint32_t>& pixels,
    const std::uint32_t color,
    const int ax, const int ay,
    const int bx, const int by
);

void draw_line_bresenham(
    std::vector<std::uint32_t>& pixels,
    const std::uint32_t color,
    const int ax, const int ay,
    const int bx, const int by
);

void draw_line_dda(
    std::vector<std::uint32_t>& pixels,
    const std::uint32_t color,
    const int ax, const int ay,
    const int bx, const int by
);

#endif