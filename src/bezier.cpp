#include "bezier.h"
#include "line.h"
#include <array>
#include <cassert>
#include <cmath>

void draw_bezier_quad_seg(SDL_Renderer* renderer, int x0, int y0, int x1, int y1, int x2, int y2)
{
    int sx = x2 - x1;
    int sy = y2 - y1;
    // Relative values for checks
    long xx = x0 - x1;
    long yy = y0 - y1;
    // Sign of gradient must not change
    assert(xx * sx <= 0 && yy * sy <= 0);

    long xy;
    double dx;
    double dy;
    double err;
    // Curvature
    double cur = (xx * sy) - (yy * sx);
    
    if ((sx * static_cast<long>(sx)) + (sy * static_cast<long>(sy)) > (xx * xx) + (yy * yy))
    {
        // Begin with longer part.
        // Swap P0 and P2.
        x2 = x0;
        x0 = sx + x1;
        y2 = y0;
        y0 = sy + y1;
        cur = -cur;
    }

    if (cur != 0) // Not a straight line
    {
        // X step direction
        xx += sx;
        sx = x0 < x2 ? 1 : -1;
        xx *= sx;
        // Y step direction
        yy += sy;
        sy = y0 < y2 ? 1 : -1;
        yy *= sy;
        // Differences 2nd degree
        xy = 2 * xx * yy;
        xx *= xx;
        yy *= yy;

        if (cur * sx * sy < 0) // Negated curvature
        {
            xx = -xx;
            yy = -yy;
            xy = -xy;
            cur = -cur;
        }

        dx = (4.0 * sy * cur * (x1 - x0)) + xx - xy;
        dy = (4.0 * sx * cur * (y0 - y1)) + yy - xy;
        xx += xx;
        yy += yy;
        err = dx + dy + xy;
        
        do
        {
            SDL_RenderDrawPoint(renderer, x0, y0);
            if (x0 == x2 && y0 == y2) // Last pixel; curve finished
                return;
            const double two_err = 2 * err;
            const bool two_err_lt_dx = two_err < dx;
            if (two_err > dy) // X step
            {
                x0 += sx;
                dx -= xy;
                dy += yy;
                err += dy;
            }
            if (two_err_lt_dx) // Y step
            {
                y0 += sy;
                dy -= xy;
                dx += xx;
                err += dx;
            }
        }
        while (dy < 0 && dx > 0); // Algorithm fails if gradient negates
    }

    // Plot remaining part to end
    draw_line_bresenham(renderer, {x0, y0}, {x2, y2});
}


void draw_bezier_quad(SDL_Renderer* renderer, int x0, int y0, int x1, int y1, int x2, int y2)
{
    int x = x0 - x1;
    int y = y0 - y1;
    double t = x0 - (2 * x1) + x2;
    double r;

    if (static_cast<long>(x) * (x2 - x1) > 0) // horizontal cut at P4?
    {
        if ((static_cast<long>(y) * (y2 - y1) > 0) && // Vertical cut at P6 too?
            (std::fabs(((y0 - (2 * y1) + y2) / t) * x) > std::abs(y))) // Which first?
        {
            // Swap points.
            // Now the horizontal cut at P4 comes first.
            x0 = x2;
            x2 = x + x1;
            y0 = y2;
            y2 = y + y1;    
        }
        t = (x0 - x1) / t;
        // By (t = P4)
        r = ((1 - t) * (((1 - t) * y0) + (2.0 * t * y1))) + (t * t * y2);
        // Gradient dP4 / dx = 0
        t = (((x0 * x2) - (x1 * x1)) * t) / (x0 - x1);
        x = std::floor(t + 0.5);
        y = std::floor(r + 0.5);
        // Intersect P3 | P0 P1
        r = (((y1 - y0) * (t - x0)) / (x1 - x0)) + y0;
        draw_bezier_quad_seg(renderer, x0, y0, x, std::floor(r + 0.5), x, y);
        // Intersect P4 | P1 P2
        r = (((y1 - y2) * (t - x2)) / (x1 - x2)) + y2;
        // P0 = P4, P1 = P8
        x1 = x;
        x0 = x1;
        y0 = y;
        y1 = std::floor(r + 0.5);
    }

    if (static_cast<long>(y0 - y1) * (y2 - y1) > 0) // Vertical cut at P6?
    {
        t = y0 - (2 * y1) + y2;
        t = (y0 - y1) / t;
        // Bx(t = P6)
        r = ((1 - t) * (((1 - t) * x0) + (2.0 * t * x1))) + (t * t * x2);
        // Gradient dP6 / dy = 0
        t = (((y0 * y2) - (y1 * y1)) * t) / (y0 - y1);
        x = std::floor(r + 0.5);
        y = std::floor(t + 0.5);
        // Intersect P6 | P0 P1
        r = (((x1 - x0) * (t - y0)) / (y1 - y0)) + x0;
        draw_bezier_quad_seg(renderer, x0, y0, std::floor(r + 0.5), y, x, y);
        // Intersect P7 | P1 P2
        r = (((x1 - x2) * (t - y2)) / (y1 - y2)) + x2;
        // P0 = P6, P1 = P7
        x0 = x;
        x1 = std::floor(r + 0.5);
        y1 = y;
        y0 = y1;
    }

    // Remaining part
    draw_bezier_quad_seg(renderer, x0, y0, x1, y1, x2, y2);
}


void draw_bezier_cubic_seg(SDL_Renderer* renderer, int x0, int y0, float x1, float y1, float x2, float y2, int x3, int y3)
{
    int f;
    int fx;
    int fy;
    int leg = 1;

    // Step direction
    int sx = x0 < x3 ? 1 : -1;
    int sy = y0 < y3 ? 1 : -1;

    float xc = -std::fabs(x0 + x1 - x2 - x3);
    float xa = xc - (4 * sx * (x1 - x2));
    float xb = sx * (x0 - x1 - x2 + x3);
    float yc = -std::fabs(y0 + y1 - y2 -y3);
    float ya = yc - (4 * sy * (y1 - y2));
    float yb = sy * (y0 - y1 - y2 + y3);
    
    double ab;
    double ac;
    double bc;
    double cb;
    double xx;
    double xy;
    double yy;
    double dx;
    double dy;
    double ex;
    double* pxy;
    double EP = 0.01;

    // Check for curve restraints:
    // Slope P0-P1 == P2-P3 and (P0-P3 == P1-P2 or no slope change) 
    assert((x1 - x0) * (x2 - x3) < EP && ((x3 - x0) * (x1 - x2) < EP || xb * xb < (xa * xc) + EP));
    assert((y1 - y0) * (y2 - y3) < EP && ((y3 - y0) * (y1 - y2) < EP || yb * yb < (ya * yc) + EP));
    
    if (xa == 0 && ya == 0) // quadratic Bezier
    {
        // New midpoint
        sx = std::floor(((3 * x1) - x0 + 1) / 2);
        sy = std::floor(((3 * y1) - y0 + 1) / 2);
        draw_bezier_quad_seg(renderer, x0, y0, sx, sy, x3, y3);
        return;
    }

    // Line lengths
    x1 = ((x1 - x0) * (x1 - x0)) + ((y1 - y0) * (y1 - y0)) + 1;
    x2 = ((x2 - x3) * (x2 - x3)) + ((y2 - y3) * (y2 - y3)) + 1;
    do // loop over both ends
    {
        ab = (xa * yb) - (xb * ya);
        ac = (xa * yc) - (xc * ya);
        bc = (xb * yc) - (xc * yb);
        // P0 part of self-intersection loop?
        ex = (ab * (ab + ac - (3 * bc))) + (ac * ac);
        // Calculate resolution
        f = ex > 0 ? 1 : std::sqrt(1 + (1024/x1));
        // Increase resolution
        ab *= f;
        ac *= f;
        bc *= f;
        ex *= f * f;

        // Init differences of 1st degree
        xy = (9 * (ab + ac + bc)) / 8;
        cb = 8 * (xa - ya);
        dx = (27 * (8*ab*(yb*yb-ya*yc)+ex*(ya+2*yb+yc)) / 64) - (ya * ya * (xy - ya));
        dy = (27 * (8*ab*(xb*xb-xa*xc)-ex*(xa+2*xb+xc)) / 64) - (xa * xa * (xy + xa));

        // Init differences of 2nd degree
        xx = 3*(3*ab*(3*yb*yb-ya*ya-2*ya*yc)-ya*(3*ac*(ya+yb)+ya*cb))/4;
        yy = 3*(3*ab*(3*xb*xb-xa*xa-2*xa*xc)-xa*(3*ac*(xa+xb)+xa*cb))/4;
        xy = xa * ya * ((6 * ab) + (6 * ac) - (3 * bc) + cb);
        ac = ya * ya;
        cb = xa * xa;
        xy = 3 * (xy + (9 * f * ((cb * yb * yc) - (xb * xc * ac))) - (18 * xb * yb * ab)) / 8;

        if (ex < 0)
        {
            // Negate values if inside self-intersection loop
            dx = -dx;
            dy = -dy;
            xx = -xx;
            yy = -yy;
            xy = -xy;
            ac = -ac;
            cb = -cb;
        }

        // Init differences of 3rd degree
        ab =  6 * ya * ac;
        ac = -6 * xa * ac;
        bc =  6 * ya * cb;
        cb = -6 * xa * cb;

        // Error of 1st step
        dx += xy;
        ex = dx + dy;
        dy += xy; 

        for (pxy = &xy, fx = fy = f; x0 != x3 && y0 != y3; )
        {
            bool should_exit_for_loop = false;
            SDL_RenderDrawPoint(renderer, x0, y0);
            do // Move sub-steps of one pixel
            {
                // Confusing values
                if (dx > *pxy || dy < *pxy)
                {
                    should_exit_for_loop = true;
                    break;
                }
                // Save value for test of y step
                y1 = (2 * ex) - dy;
                if (2 * ex >= dx) // X sub-step
                {
                    fx--;
                    dx += xx;
                    ex += dx;
                    xy += ac;
                    dy += xy;
                    yy += bc;
                    xx += ab;
                }
                if (y1 <= 0) // Y sub-step
                {
                    fy--;
                    dy += yy;
                    ex += dy;
                    xy += bc;
                    dx += xy;
                    xx += ac;
                    yy += cb;
                }
            }
            while (fx > 0 && fy > 0); // Pixel complete?

            if (should_exit_for_loop)
                break;

            if (2 * fx <= f) // X step
            {
                x0 += sx;
                fx += f;
            }
            if (2 * fy <= f) // Y step
            {
                y0 += sy;
                fy += f;
            }
            if (pxy == &xy && dx < 0 && dy > 0) // Pixel ahead is valid
                pxy = &EP;
        }

        // Swap legs
        xx = x0;
        x0 = x3;
        x3 = xx;
        sx = -sx;
        xb = -xb;
        yy = y0;
        y0 = y3;
        y3 = yy;
        sy = -sy;
        yb = -yb;
        x1 = x2;
    }
    while (leg--); // Try other end

    // Remaining part in case of cusp or crunode
    draw_line_bresenham(renderer, {x0, y0}, {x3, y3});
}


void draw_bezier_cubic(SDL_Renderer* renderer, int x0, int y0, float x1, float y1, float x2, float y2, int x3, int y3)
{
    int n = 0;
    int i = 0;
    long xc = x0 + x1 - x2 - x3;
    long xa = xc - (4 * (x1 - x2));
    long xb = x0 - x1 - x2 + x3;
    long xd = xb + (4 * (x1 + x2));
    long yc = y0 + y1 - y2 - y3;
    long ya = yc - (4 * (y1 - y2));
    long yb = y0 - y1 - y2 + y3;
    long yd = yb + (4 * (y1 + y2));
    float fx0 = x0;
    float fx1;
    float fx2;
    float fx3;
    float fy0 = y0;
    float fy1;
    float fy2;
    float fy3;
    double t1 = (xb * xb) - (xa * xc);
    double t2;
    std::array<double, 5> t;

    // Sub-divide curve at gradient sign changes
    if (xa == 0) // Horizontal
    {
        // One change
        if (std::abs(xc) < 2 * std::abs(xb))
            t.at(n++) = xc / (2.0 * xb);
    }
    else if (t1 > 0.0)
    {
        // Two changes
        t2 = std::sqrt(t1);
        t1 = (xb - t2) / xa;
        if (std::fabs(t1) < 1.0)
            t.at(n++) = t1;
        t1 = (xb + t2) / xa;
        if (std::fabs(t1) < 1.0)
            t.at(n++) = t1;
    }
    t1 = (yb * yb) - (ya * yc);
    if (ya == 0) // Vertical
    {
        // One change
        if (std::abs(yc) < 2 * std::abs(yb))
            t.at(n++) = yc / (2.0 * yb);
    }
    else if (t1 > 0.0)
    {
        // Two changes
        t2 = std::sqrt(t1);
        t1 = (yb - t2) / ya;
        if (std::fabs(t1) < 1.0)
            t.at(n++) = t1;
        t1 = (yb + t2) / ya;
        if (std::fabs(t1) < 1.0)
            t.at(n++) = t1;
    }

    // Bubble sort of 4 points
    //std::sort(t.begin(), t.end() - 1);
    for (i = 1; i < n; i++)
    {
        t1 = t.at(i-1);
        if (t1 > t.at(i))
        {
            t.at(i-1) = t.at(i);
            t.at(i) = t1;
            i = 0;
        }
    }

    // Begin / end point
    t1 = -1.0;
    t.at(n) = 1.0;

    // Plot each segment separately
    for (i = 0; i <= n; i++)
    {
        // Sub-divide at t[i-1], t[i]
        t2 = t.at(i);
        fx1 = (t1*(t1*xb-2*xc)-t2*(t1*(t1*xa-2*xb)+xc)+xd)/8-fx0;
        fy1 = (t1*(t1*yb-2*yc)-t2*(t1*(t1*ya-2*yb)+yc)+yd)/8-fy0;
        fx2 = (t2*(t2*xb-2*xc)-t1*(t2*(t2*xa-2*xb)+xc)+xd)/8-fx0;
        fy2 = (t2*(t2*yb-2*yc)-t1*(t2*(t2*ya-2*yb)+yc)+yd)/8-fy0;
        fx3 = (t2*(t2*(3*xb-t2*xa)-3*xc)+xd)/8;
        fx0 -= fx3;
        fy3 = (t2*(t2*(3*yb-t2*ya)-3*yc)+yd)/8;
        fy0 -= fy3;
        // Scale bounds to int
        x3 = std::floor(fx3 + 0.5);
        y3 = std::floor(fy3 + 0.5);
        if (fx0 != 0.0)
        {
            fx0 = (x0 - x3) / fx0;
            fx1 *= fx0;
            fx2 *= fx0;
        }
        if (fy0 != 0.0)
        {
            fy0 = (y0 - y3) / fy0;
            fy1 *= fy0;
            fy2 *= fy0;
        }
        if (x0 != x3 || y0 != y3) // Segment t1 - t2
            draw_bezier_cubic_seg(renderer, x0, y0, x0 + fx1, y0 + fy1, x0 + fx2, y0 + fy2, x3, y3);
        x0 = x3;
        y0 = y3;
        fx0 = fx3;
        fy0 = fy3;
        t1 = t2;
    }
}