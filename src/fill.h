#ifndef FILL_H
#define FILL_H

#include <cstdint>
#include <vector>

struct BoundingRect {
    unsigned int x_min;
    unsigned int y_min;
    unsigned int x_max;
    unsigned int y_max;
};

BoundingRect get_bounding_rect(const std::vector<std::uint32_t> pixels, const std::uint32_t border_color);

void scanline_fill_area(
    std::vector<std::uint32_t>& pixels,
    const unsigned int x_min, const unsigned int y_min,
    const unsigned int x_max, const unsigned int y_max,
    const std::uint32_t color
);

void scanline_fill(std::vector<std::uint32_t>& pixels, const std::uint32_t color);

void flood_fill_stack(
    std::vector<std::uint32_t>& pixels,
    const std::uint32_t color,
    const int x, const int y
);

void flood_fill_recursive(
    std::vector<std::uint32_t>& pixels,
    const std::uint32_t color,
    const int x, const int y
);

#endif
