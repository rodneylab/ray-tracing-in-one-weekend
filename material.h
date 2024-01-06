#ifndef MATERIAL_H
#define MATERIAL_H

#include "colour.h"
#include "ray.h"
#include "utility.h"

class HitRecord;

class Material
{
public:
    virtual ~Material() = default;

    virtual bool scatter(const Ray &ray_in,
                         const HitRecord &record,
                         Colour &attenuation,
                         Ray &scattered) const = 0;
};

class Lambertian : public Material
{
public:
    Lambertian(const Colour &albedo) : _albedo(albedo)
    {
    }

    bool scatter(const Ray & /*ray_in*/,
                 const HitRecord &record,
                 Colour &attenuation,
                 Ray &scattered) const override
    {
        Vec3 scatter_direction{record._normal + random_unit_vector()};

        // catch degenerate scatter direction
        if (scatter_direction.near_zero())
        {
            scatter_direction = record._normal;
        }

        scattered = Ray{record._point, scatter_direction};
        attenuation = _albedo;
        return true;
    }

private:
    Colour _albedo;
};

class Metal : public Material
{
public:
    Metal(const Colour &albedo, double fuzz)
        : _albedo(albedo), _fuzz(fuzz < 1 ? fuzz : 1)
    {
    }

    bool scatter(const Ray &ray_in,
                 const HitRecord &record,
                 Colour &attenuation,
                 Ray &scattered) const override
    {
        Vec3 reflected{
            reflect(unit_vector(ray_in.direction()), record._normal)};
        scattered =
            Ray{record._point, reflected + _fuzz * random_unit_vector()};
        attenuation = _albedo;
        return (dot(scattered.direction(), record._normal) > 0);
    }

private:
    Colour _albedo;
    double _fuzz;
};

// Dielectric materials, such as glass and water, both reflect and refract incident light.
class Dielectric : public Material
{
public:
    Dielectric(double index_of_refraction)
        : _refraction_index(index_of_refraction)
    {
    }

    bool scatter(const Ray &ray_in,
                 const HitRecord &record,
                 Colour &attenuation,
                 Ray &scattered) const override
    {
        attenuation = {Colour(1.0, 1.0, 1.0)};
        const double refraction_ratio{
            record._front_face ? (1.0 / _refraction_index) : _refraction_index};

        Vec3 unit_direction{unit_vector(ray_in.direction())};
        const double cos_theta{fmin(dot(-unit_direction, record._normal), 1.0)};
        const double sin_theta{sqrt(1.0 - cos_theta * cos_theta)};

        const bool cannot_refract{refraction_ratio * sin_theta > 1.0};
        Vec3 direction;

        if (cannot_refract)
        {
            direction = reflect(unit_direction, record._normal);
        }
        else
        {
            direction =
                refract(unit_direction, record._normal, refraction_ratio);
        }

        scattered = Ray{record._point, direction};

        return true;
    }

private:
    double _refraction_index;
};

#endif
