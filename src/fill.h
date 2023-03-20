#ifndef FILL_H
#define FILL_H

#include <array>
#include <vector>

std::array<unsigned int, 4> get_bounding_rect(std::vector<uint32_t> pixels,
    const unsigned int width, const unsigned int height, uint32_t border_color);
void scanline_fill_area(std::vector<std::uint32_t>& pixels,
    const unsigned int width, const unsigned int height,
    const unsigned int x_min, const unsigned int y_min,
    const unsigned int x_max, const unsigned int y_max,
    const std::uint32_t color);
void scanline_fill(std::vector<std::uint32_t>& pixels,
    const unsigned int width, const unsigned int height,
    const std::uint32_t color);
void flood_fill(std::vector<std::uint32_t>& pixels,
    const unsigned int width, const unsigned int height,
    const std::uint32_t color, const int x, const int y);

#endif