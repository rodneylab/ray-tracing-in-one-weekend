#ifndef HITTABLE_H
#define HITTABLE_H

#include "interval.h"
#include "ray.h"

class Material;

class HitRecord
{
public:
    Point3 _point;
    Vec3 _normal;
    std::shared_ptr<Material> _material;
    double _t_interval = {};
    bool _front_face = {};

    void set_face_normal(const Ray &ray, const Vec3 &outward_normal)
    {
        // Sets the hit record normal vector
        // NOTE: we assume the parameter `outward_normal` has unit length
        _front_face = dot(ray.direction(), outward_normal) < 0;
        _normal = _front_face ? outward_normal : -outward_normal;
    }
};

class Hittable
{
public:
    Hittable() = default;
    // Hittable(Hittable &&) = delete;
    // Hittable &operator=(const Hittable &) = default;
    // Hittable &operator=(Hittable &&) = delete;
    virtual ~Hittable() = default;

    virtual bool hit(const Ray &ray, Interval ray_t, HitRecord &rec) const = 0;
};

#endif
