#ifndef COLOUR_H
#define COLOUR_H

#include "interval.h"

#include <iostream>

#include "vec3.h"

using Colour = Vec3;

inline double linear_to_gamma(double linear_component)
{
    return sqrt(linear_component);
}

void write_colour(std::ostream &out, Colour pixel_colour, int samples_per_pixel)
{
    double r{pixel_colour.x()};
    double g{pixel_colour.y()};
    double b{pixel_colour.z()};

    const double scale{1.0 / samples_per_pixel};
    r *= scale;
    g *= scale;
    b *= scale;

    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    static const Interval intensity{0.000, 0.999};
    constexpr int kMaxIntensity{256};
    out << static_cast<int>(kMaxIntensity * intensity.clamp(r)) << ' '
        << static_cast<int>(kMaxIntensity * intensity.clamp(g)) << ' '
        << static_cast<int>(kMaxIntensity * intensity.clamp(b)) << '\n';
}

#endif
