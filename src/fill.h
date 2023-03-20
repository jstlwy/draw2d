#ifndef FILL_H
#define FILL_H

#include <array>
#include <vector>

std::array<unsigned int, 4> get_bounding_rect(std::vector<uint32_t> pixels,
    const unsigned int width, const unsigned int height, uint32_t border_color);
void scanline_fill(std::vector<std::uint32_t>& pixels,
    const unsigned int width, const unsigned int height,
    const std::uint32_t color);
void flood_fill(std::vector<std::uint32_t>& pixels,
    const unsigned int width, const unsigned int height,
    const std::uint32_t color, const int x, const int y);

#endif