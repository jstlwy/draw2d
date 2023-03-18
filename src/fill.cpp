#include "fill.h"

void flood_fill(std::vector<std::uint32_t>& pixels,
    const unsigned int width, const unsigned int height,
    const std::uint32_t color, const int x, const int y)
{
    if (x < 0 || x >= width || y < 0 || y >= height)
        return;

    const unsigned int row = y * width;
    if (pixels.at(row + x) != color)
    {
        pixels.at(row + x) = color;
        flood_fill(pixels, width, height, color, x + 1, y);
        flood_fill(pixels, width, height, color, x - 1, y);
        flood_fill(pixels, width, height, color, x, y + 1);
        flood_fill(pixels, width, height, color, x, y - 1);
    }
}