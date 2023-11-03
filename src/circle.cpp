#include "circle.h"
#include "constants.h"

void plot_circle_points(
    std::vector<std::uint32_t>& pixels,
    const std::uint32_t color,
    const int cx, const int cy,
    const int x, const int y)
{
    /*
       Normal order (x, y):
       cx + x, cy + y
       cx - x, cy + y
       cx + x, cy - y
       cx - x, cy - y
       cx + y, cy + x
       cx - y, cy + x
       cx + y, cy - x
       cx - y, cy - x
     */

    // When accessing the array, the coordinates are in reverse order:
    // pixels.at((y * SCREEN_WIDTH) + x) = color;
    const int y_cy_add_y = (cy + y) * SCREEN_WIDTH;
    const int y_cy_sub_y = (cy - y) * SCREEN_WIDTH; 
    const int y_cy_add_x = (cy + x) * SCREEN_WIDTH;
    const int y_cy_sub_x = (cy - x) * SCREEN_WIDTH;
    const int x_cx_add_x = cx + x;
    const int x_cx_sub_x = cx - x;
    const int x_cx_add_y = cx + y;
    const int x_cx_sub_y = cx - y;
    pixels[y_cy_add_y + x_cx_add_x] = color;
    pixels[y_cy_add_y + x_cx_sub_x] = color;
    pixels[y_cy_sub_y + x_cx_add_x] = color;
    pixels[y_cy_sub_y + x_cx_sub_x] = color;
    pixels[y_cy_add_x + x_cx_add_y] = color;
    pixels[y_cy_add_x + x_cx_sub_y] = color;
    pixels[y_cy_sub_x + x_cx_add_y] = color;
    pixels[y_cy_sub_x + x_cx_sub_y] = color;
}

void draw_circle_midpoint(
    std::vector<std::uint32_t>& pixels,
    const std::uint32_t color,
    const int cx, const int cy,
    const int radius)
{
    int x = 0;
    int y = radius;
    int p = 1 - radius;

    plot_circle_points(pixels, color, cx, cy, x, y);

    while (x < y) {
        x++;
        if (p < 0) {
            p += (2 * x) + 1;
        } else {
            y--;
            p += (2 * (x - y)) + 1;	
        }
        plot_circle_points(pixels, color, cx, cy, x, y);
    }
}
