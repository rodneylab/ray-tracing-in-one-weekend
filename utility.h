#ifndef UTILTIY_H
#define UTILTIY_H

#include <cmath>
#include <limits>
#include <memory>
#include <random>

namespace constants
{
inline constexpr double kInfinity{std::numeric_limits<double>::infinity()};
inline constexpr double kPi{3.1415926535897932385};
} // namespace constants

inline double degrees_to_radians(double degrees)
{
    // NOLINTNEXTLINE(readability-magic-numbers)
    return degrees * constants::kPi / 180.0;
}

inline double random_double()
{
    // returns a random, real in [0,1)
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double random_double(double min_included, double max_excluded)
{
    return min_included + (max_excluded - min_included) * random_double();
}

#endif
