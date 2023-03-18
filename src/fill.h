#ifndef FILL_H
#define FILL_H

#include <vector>

void flood_fill(std::vector<std::uint32_t>& pixels,
    const unsigned int width, const unsigned int height,
    const std::uint32_t color, const int x, const int y);

#endif