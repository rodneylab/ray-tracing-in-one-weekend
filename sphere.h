#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class Sphere : public Hittable
{
public:
    Sphere(Point3 centre, double radius, std::shared_ptr<Material> material)
        : _centre(centre), _radius(radius), _material(material)
    {
    }

    bool hit(const Ray &ray, Interval ray_t, HitRecord &record) const override
    {
        const Vec3 sphere_origin_displacement{ray.origin() - _centre};
        const double quadratic_coefficient_a{ray.direction().length_squared()};
        const double half_quadratic_coefficient_b{
            dot(sphere_origin_displacement, ray.direction())};
        const double quadratic_coefficient_c{
            sphere_origin_displacement.length_squared() - _radius * _radius};

        const double discriminant{
            half_quadratic_coefficient_b * half_quadratic_coefficient_b -
            quadratic_coefficient_a * quadratic_coefficient_c};
        if (discriminant < 0)
        {
            return false;
        }
        const double discriminant_sqrt{sqrt(discriminant)};

        // Find the nearest root that lines on the acceptable range
        double root = (-half_quadratic_coefficient_b - discriminant_sqrt) /
                      quadratic_coefficient_a;
        if (!ray_t.surrounds(root))
        {
            root = (-half_quadratic_coefficient_b + discriminant_sqrt) /
                   quadratic_coefficient_a;
            if (!ray_t.surrounds(root))
            {
                return false;
            }
        }

        record._t_interval = root;
        record._point = ray.at(record._t_interval);
        const Vec3 outward_normal{(record._point - _centre) / _radius};
        record.set_face_normal(ray, outward_normal);
        record._material = _material;

        return true;
    }

private:
    Point3 _centre;
    double _radius;
    std::shared_ptr<Material> _material;
};

#endif
