#include "fill.h"

std::array<unsigned int, 4> get_bounding_rect(std::vector<uint32_t> pixels,
    const unsigned int width, const unsigned int height, uint32_t border_color)
{
	unsigned int x_min = width;
	unsigned int y_min = height;
    unsigned int x_max = 0;
    unsigned int y_max = 0;
    for (unsigned int y = 0; y < height; y++)
    {
		const unsigned int row = y * width;
        for (unsigned int x = 0; x < width; x++)
        {
			const unsigned int i = row + x;
            if (pixels.at(i) == border_color)
            {
                x_min = x < x_min ? x : x_min;
				y_min = y < y_min ? y : y_min;
                x_max = x > x_max ? x : x_max;
                y_max = y;
                x = x_max;
            }
        }
    }
    return {x_min, y_min, x_max, y_max};
}

void scanline_fill(std::vector<std::uint32_t>& pixels,
    const unsigned int width, const unsigned int height,
    const std::uint32_t color)
{
    std::array<unsigned int, 4> boundaries = get_bounding_rect(pixels, width, height, color);
    const unsigned int x_min = boundaries.at(0);
    const unsigned int y_min = boundaries.at(1);
    const unsigned int x_max = boundaries.at(2);
    const unsigned int y_max = boundaries.at(3);
    for (unsigned int y = y_min; y < y_max; y++)
    {
		const unsigned int row = y * width;
        const unsigned int row_start = row + x_min;
        const unsigned int row_end = row + x_max;
        unsigned int line_start = row_start;
        while (pixels.at(line_start) != color && line_start < row_end)
        {
            line_start++;
        }
        unsigned int line_end = row_end;
        while (pixels.at(line_end) != color && line_end > row_start)
        {
            line_end--;
        }
        if (line_start >= line_end)
            continue;
        for (unsigned int i = line_start + 1; i < line_end; i++)
        {
            pixels.at(i) = color;
        }
    }
}

void flood_fill(std::vector<std::uint32_t>& pixels,
    const unsigned int width, const unsigned int height,
    const std::uint32_t color, const int x, const int y)
{
    if (x < 0 || x >= width || y < 0 || y >= height)
        return;

    const int row = y * width;
    if (pixels.at(row + x) != color)
    {
        pixels.at(row + x) = color;
        flood_fill(pixels, width, height, color, x + 1, y);
        flood_fill(pixels, width, height, color, x - 1, y);
        flood_fill(pixels, width, height, color, x, y + 1);
        flood_fill(pixels, width, height, color, x, y - 1);
    }
}