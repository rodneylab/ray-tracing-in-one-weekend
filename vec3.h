#ifndef VEC3_H
#define VEC3_H

#include "utility.h"

#include <array>
#include <cmath>
#include <iostream>

class Vec3
{
public:
    std::array<double, 3> e;

    Vec3() : e{0, 0, 0}
    {
    }

    Vec3(double e0_value, double e1_value, double e2_value)
        : e{e0_value, e1_value, e2_value}
    {
    }

    [[nodiscard]] double x() const
    {
        return e[0];
    }

    [[nodiscard]] double y() const
    {
        return e[1];
    }

    [[nodiscard]] double z() const
    {
        return e[2];
    }

    Vec3 operator-() const
    {
        return {-e[0], -e[1], -e[2]};
    }

    double operator[](int i_value) const
    {
        return e[static_cast<size_t>(i_value)];
    }

    double &operator[](int i_value)
    {
        return e[static_cast<size_t>(i_value)];
    }

    Vec3 &operator+=(const Vec3 &v_value)
    {
        e[0] += v_value.e[0];
        e[1] += v_value.e[1];
        e[2] += v_value.e[2];

        return *this;
    }

    Vec3 &operator*=(double t_value)
    {
        e[0] *= t_value;
        e[1] *= t_value;
        e[2] *= t_value;

        return *this;
    }

    Vec3 &operator/=(double t_value)
    {
        return *this *= 1 / t_value;
    }

    [[nodiscard]] double length() const
    {
        return std::sqrt(length_squared());
    }

    [[nodiscard]] double length_squared() const
    {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    [[nodiscard]] bool near_zero() const
    {
        // return true is the vector is close to zero in all dimensions
        constexpr double kSmall{1e-8};
        return (fabs(e[0]) < kSmall) && (fabs(e[1])) < kSmall &&
               (fabs(e[2])) < kSmall;
    }

    static Vec3 random()
    {
        return Vec3{random_double(), random_double(), random_double()};
    }

    static Vec3 random(double min_included, double max_excluded)
    {
        return Vec3{random_double(min_included, max_excluded),
                    random_double(min_included, max_excluded),
                    random_double(min_included, max_excluded)};
    }
};


// point3 is just an alias for vec3, but useful for geometric clarity in the code.
using Point3 = Vec3;

// Vector utility functions
inline std::ostream &operator<<(std::ostream &out, const Vec3 &v_value)
{
    return out << v_value.e[0] << ' ' << v_value.e[1] << ' ' << v_value.e[2];
}

inline Vec3 operator+(const Vec3 &u_value, const Vec3 &v_value)
{
    return {u_value.e[0] + v_value.e[0],
            u_value.e[1] + v_value.e[1],
            u_value.e[2] + v_value.e[2]};
}

inline Vec3 operator-(const Vec3 &u_value, const Vec3 &v_value)
{
    return {u_value.e[0] - v_value.e[0],
            u_value.e[1] - v_value.e[1],
            u_value.e[2] - v_value.e[2]};
}

inline Vec3 operator*(const Vec3 &u_value, const Vec3 &v_value)
{
    return {u_value.e[0] * v_value.e[0],
            u_value.e[1] * v_value.e[1],
            u_value.e[2] * v_value.e[2]};
}

inline Vec3 operator*(double t_value, const Vec3 &v_value)
{
    return {t_value * v_value.e[0],
            t_value * v_value.e[1],
            t_value * v_value.e[2]};
}

inline Vec3 operator*(const Vec3 &v_value, double t_value)
{
    return t_value * v_value;
}

inline Vec3 operator/(const Vec3 &v_value, double t_value)
{
    return (1 / t_value) * v_value;
}

inline double dot(const Vec3 &u_value, const Vec3 &v_value)
{
    return u_value.e[0] * v_value.e[0] + u_value.e[1] * v_value.e[1] +
           u_value.e[2] * v_value.e[2];
}

inline Vec3 cross(const Vec3 &u_value, const Vec3 &v_value)
{
    return {u_value.e[1] * v_value.e[2] - u_value.e[2] * v_value.e[1],
            u_value.e[2] * v_value.e[0] - u_value.e[0] * v_value.e[2],
            u_value.e[0] * v_value.e[1] - u_value.e[1] * v_value.e[0]};
}

inline Vec3 unit_vector(Vec3 v_value)
{
    return v_value / v_value.length();
}

inline Vec3 random_in_unit_disc()
{
    while (true)
    {
        Vec3 result{
            Vec3{random_double(-1.0, 1.0), random_double(-1.0, 1.0), 0.0}};
        if (result.length_squared() < 1.0)
        {
            return result;
        }
    }
}

inline Vec3 random_in_unit_sphere()
{
    while (true)
    {
        Vec3 vector{Vec3::random(-1, 1)};
        if (vector.length_squared() < 1)
        {
            return vector;
        }
    }
}

inline Vec3 random_unit_vector()
{
    return unit_vector(random_in_unit_sphere());
}

inline Vec3 random_on_hemisphere(const Vec3 &normal)
{
    Vec3 on_unit_sphere{random_unit_vector()};
    // check if in the same hemisphere as the normal
    if (dot(on_unit_sphere, normal) > 0.0)
    {
        return on_unit_sphere;
    }
    return -on_unit_sphere;
}

inline Vec3 reflect(const Vec3 &vector, const Vec3 &normal)
{
    return {vector - 2 * dot(vector, normal) * normal};
}

inline Vec3 refract(const Vec3 &incident_ray_uv,
                    const Vec3 &normal,
                    double eta_i_over_eta_j)
{
    const double cos_theta{fmin(dot(-incident_ray_uv, normal), 1.0)};
    const Vec3 ray_out_perpendicular{eta_i_over_eta_j *
                                     (incident_ray_uv + cos_theta * normal)};
    const Vec3 ray_out_parallel{
        -sqrt(fabs(1.0 - ray_out_perpendicular.length_squared())) * normal};

    return ray_out_perpendicular + ray_out_parallel;
}

#endif
