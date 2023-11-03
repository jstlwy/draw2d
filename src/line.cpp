#include "line.h"
#include <cmath>
#include "constants.h"

void draw_line_zingl(
    std::vector<std::uint32_t>& pixels,
    const std::uint32_t color,
    const int ax, const int ay,
    const int bx, const int by)
{
    const int dx = std::abs(bx - ax);
    const int sx = ax < bx ? 1 : -1;
    const int dy = -std::abs(by - ay);
    const int sy = ay < by ? 1 : - 1;

    int x = ax;
    int y = ay;
    int err = dx + dy;

    while (true) {
        pixels.at((y * SCREEN_WIDTH) + x) = color;
        const int e2 = 2 * err;
        if (e2 >= dy) {
            if (x == bx) {
                break;
            }
            err += dy;
            x += sx;
        }
        if (e2 <= dx) {
            if (y == by) {
                break;
            }
            err += dx;
            y += sy;
        }
    }
}

void draw_line_bresenham(
    std::vector<std::uint32_t>& pixels,
    const std::uint32_t color,
    int ax, int ay,
    int bx, int by)
{
    if (ax < 0 || bx < 0 || ay < 0 || by < 0 || ax >= SCREEN_WIDTH || bx >= SCREEN_WIDTH || ay >= SCREEN_HEIGHT || by >= SCREEN_HEIGHT) {
        return;
    }

    const int dx = bx > ax ? bx - ax : ax - bx;
    const int dy = by > ay ? by - ay : ay - by;

    if (dx == 0) {
        // Vertical lines
        if (ay > by) {
            std::swap(ay, by);
        }
        const int row_end = by * SCREEN_WIDTH;
        for (int row = ay * SCREEN_WIDTH; row <= row_end; row += SCREEN_WIDTH) {
            pixels[row + ax] = color;
        }
    } else if (dy == 0) {
        // Horizontal lines
        if (ax > bx) {
            std::swap(ax, bx);
        }
        const int row = ay * SCREEN_WIDTH;
        std::fill(pixels.begin() + (row + ax), pixels.begin() + (row + bx + 1), color);
    } else if (dx == dy) {
        // Slope = 1: Perfectly diagonal lines
        if (ax > bx) {
            std::swap(ax, bx);
            std::swap(ay, by);
        }
        const int srow = ay < by ? SCREEN_WIDTH : -SCREEN_WIDTH;
        for (int x = ax, row = ay * SCREEN_WIDTH; x <= bx; x++, row += srow) {
            pixels[row + x] = color;
        }
    } else if (dx > dy) {
        // Slope < 1: Gradual lines
        if (ax > bx) {
            std::swap(ax, bx);
            std::swap(ay, by);
        }
        const int srow = ay < by ? SCREEN_WIDTH : -SCREEN_WIDTH;

        const int two_dy = 2 * dy;
        const int two_diff_dy_dx = 2 * (dy - dx);
        int p = two_dy - dx;

        for (int x = ax, row = ay * SCREEN_WIDTH; x <= bx; x++) {
            pixels[row + x] = color;
            const int mask = p >> 31;
            const int nMask = ~mask;
            p += (two_dy & mask) + (two_diff_dy_dx & nMask);
            row += srow & nMask;
        }
    } else {
        // Slope > 1: Steep lines
        if (ay > by) {
            std::swap(ax, bx);
            std::swap(ay, by);
        }
        const int sx = ax < bx ? 1 : -1;
        const int row_end = by * SCREEN_WIDTH;

        const int two_dx = 2 * dx;
        const int two_diff_dx_dy = 2 * (dx - dy);
        int p = two_dx - dy;

        for (int x = ax, row = ay * SCREEN_WIDTH; row <= row_end; row += SCREEN_WIDTH) {
            pixels[row + x] = color;
            const int mask = p >> 31;
            const int nMask = ~mask;
            p += (two_dx & mask) + (two_diff_dx_dy & nMask);
            x += sx & nMask;
        }
    }
}


void draw_line_dda(
    std::vector<std::uint32_t>& pixels,
    const std::uint32_t color,
    const int ax, const int ay,
    const int bx, const int by)
{
    const int dx = bx - ax;
    const int dy = by - ay;

    const int abs_dx = std::abs(dx);
    const int abs_dy = std::abs(dy);
    const int steps = abs_dx > abs_dy ? abs_dx : abs_dy;

    double x = static_cast<double>(ax);
    double y = static_cast<double>(ay);
    const double x_incr = dx / static_cast<double>(steps);
    const double y_incr = dy / static_cast<double>(steps);

    pixels.at((std::round(y) * SCREEN_WIDTH) + std::round(x)) = color;
    for (int k = 0; k < steps; k++) {
        x += x_incr;
        y += y_incr;
        pixels.at((std::round(y) * SCREEN_WIDTH) + std::round(x)) = color;
    }
}
