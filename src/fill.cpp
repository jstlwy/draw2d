#include "fill.h"
#include "constants.h"
#include <stack>

BoundingRect get_bounding_rect(const std::vector<std::uint32_t> pixels, const std::uint32_t border_color)
{
    unsigned int x_min = SCREEN_WIDTH;
    unsigned int y_min = SCREEN_HEIGHT;
    unsigned int x_max = 0;
    unsigned int y_max = 0;
    for (unsigned int y = 0; y < SCREEN_HEIGHT; y++) {
        const unsigned int row = y * SCREEN_WIDTH;
        for (unsigned int x = 0; x < SCREEN_WIDTH; x++) {
            if (pixels.at(row + x) == border_color) {
                if (x < x_min) {
                    x_min = x;
                }
                if (y < y_min) {
                    y_min = y;
                }
                if (x > x_max) {
                    x_max = x;
                }
                y_max = y;
                x = x_max;
            }
        }
    }
    return {x_min, y_min, x_max, y_max};
}

void scanline_fill_area(
    std::vector<std::uint32_t>& pixels,
    const unsigned int x_min, const unsigned int y_min,
    const unsigned int x_max, const unsigned int y_max,
    const std::uint32_t color)
{
    for (unsigned int y = y_min; y < y_max; y++) {
        const unsigned int row = y * SCREEN_WIDTH;
        const unsigned int row_start = row + x_min;
        const unsigned int row_end = row + x_max;
        unsigned int line_start = row_start;
        while (pixels.at(line_start) != color && line_start < row_end) {
            line_start++;
        }
        unsigned int line_end = row_end;
        while (pixels.at(line_end) != color && line_end > row_start) {
            line_end--;
        }
        if (line_start >= line_end) {
            continue;
        }
        for (unsigned int i = line_start + 1; i < line_end; i++) {
            pixels.at(i) = color;
        }
    }
}

void scanline_fill(std::vector<std::uint32_t>& pixels, const std::uint32_t color)
{
    BoundingRect br = get_bounding_rect(pixels, color);
    scanline_fill_area(pixels, br.x_min, br.y_min, br.x_max, br.y_max, color);
}

void flood_fill_stack(
    std::vector<std::uint32_t>& pixels,
    const std::uint32_t color,
    const int x, const int y)
{
    if ((x < 0) || (x >= SCREEN_WIDTH) || (y < 0) || (y >= SCREEN_HEIGHT)) {
        return;
    }

    std::stack<int> pointStack;
    pointStack.push((y * SCREEN_WIDTH) + x);
    while (!pointStack.empty()) {
        const int p = pointStack.top();
        pointStack.pop();
        if ((p >= 0) && (p < NUM_PIXELS) && (pixels.at(p) != color)) {
            pixels.at(p) = color;
            pointStack.push(p - 1);
            pointStack.push(p + 1);
            pointStack.push(p - SCREEN_WIDTH);
            pointStack.push(p + SCREEN_WIDTH);
        }
    }
}

void flood_fill_recursive(
    std::vector<std::uint32_t>& pixels,
    const std::uint32_t color,
    const int x, const int y)
{
    if ((x < 0) || (x >= SCREEN_WIDTH) || (y < 0) || (y >= SCREEN_HEIGHT)) {
        return;
    }

    const int row = y * SCREEN_WIDTH;
    if (pixels.at(row + x) != color) {
        pixels.at(row + x) = color;
        flood_fill_recursive(pixels, color, x + 1, y);
        flood_fill_recursive(pixels, color, x - 1, y);
        flood_fill_recursive(pixels, color, x, y + 1);
        flood_fill_recursive(pixels, color, x, y - 1);
    }
}
