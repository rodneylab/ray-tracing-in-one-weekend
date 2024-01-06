#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class Ray
{
public:
    Ray() = default;

    Ray(const Point3 &origin, const Vec3 &direction)
        : _origin(origin), _direction(direction)
    {
    }

    [[nodiscard]] Point3 origin() const
    {
        return _origin;
    }

    [[nodiscard]] Vec3 direction() const
    {
        return _direction;
    }

    [[nodiscard]] Point3 at(double t_value) const
    {
        return _origin + t_value * _direction;
    }

private:
    Point3 _origin;
    Vec3 _direction;
};

#endif
