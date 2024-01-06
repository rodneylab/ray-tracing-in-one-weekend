#ifndef CAMERA_H
#define CAMERA_H

#include "colour.h"
#include "hittable.h"
#include "material.h"
#include "utility.h"

#include <format>

class Camera
{
public:
    double _aspect_ratio = 1.0;
    int _image_width = 100;
    int _samples_per_pixel = 10;
    int _max_depth = 10; // maximum nuber of ray bounces into scene

    double _vertical_fov = 90.0; // vertical field of view
    Point3 _look_from = Point3{0.0, 0.0, -1.0};
    Point3 _look_at = Point3{0.0, 0.0, 0.0};
    Point3 _vup = Vec3{0.0, 1.0, 0.0}; // view up

    double _defocus_angle = 0.0;
    double _focus_dist = 10.0;

    void render(const Hittable &world)
    {
        initialise();

        std::cout << std::format("P3\n {} {}\n255\n",
                                 _image_width,
                                 _image_height);

        //std::cout << "P3\n " << _image_width << ' ' << _image_height
        //        << "\n255\n";

        for (int j{0}; j < _image_height; ++j)
        {
            std::clog << std::format("\rScanlines remaining: {} ",
                                     _image_height - j)
                      << std::flush;
            //std::clog << "\rScanlines remaining: " << _image_height - j << ' '
            //         << std::flush;
            for (int i{0}; i < _image_width; ++i)
            {
                Colour pixel_colour{0.0, 0.0, 0.0};
                for (int sample{0}; sample < _samples_per_pixel; ++sample)
                {
                    const Ray ray{get_ray(i, j)};
                    pixel_colour += ray_colour(ray, _max_depth, world);
                }
                write_colour(std::cout, pixel_colour, _samples_per_pixel);
            }
        }
        std::clog << "\rDone.                  \n";
    }

private:
    int _image_height;   // rendered image height
    Point3 _centre;      // camera centre
    Point3 _pixel00_loc; // location of pixel 0,0
    Vec3 _pixel_delta_u; // offset to pixel to the right
    Vec3 _pixel_delta_v; // offset to pixel below
    Vec3 _u;             // camera frame basis vectors
    Vec3 _v;
    Vec3 _w;
    Vec3 _defocus_disc_u; // defocus disc horizontal radius
    Vec3 _defocus_disc_v; // defocus disc vertical radius


    void initialise()
    {
        _image_height = {static_cast<int>(_image_width / _aspect_ratio)};
        _image_height = {(_image_height < 1) ? 1 : _image_height};

        _centre = _look_from;

        // Determine viewport dimensions
        const double theta{degrees_to_radians(_vertical_fov)};
        const double height{tan(theta / 2.0)};
        const double viewport_height{2.0 * height * _focus_dist};
        const double viewport_width{
            viewport_height *
            (static_cast<double>(_image_width) / _image_height)};

        // Calculate the u,v,w unit basis vectors for the camera coordinate frame.
        _w = unit_vector(_look_from - _look_at);
        _u = unit_vector(cross(_vup, _w));
        _v = cross(_w, _u);

        // Calculate the vectors across the horizontal and down the vertical viewport edges
        const Vec3 viewport_u{viewport_width * _u};
        const Vec3 viewport_v{viewport_height * -_v};

        // Calculate the horizontal and vertical delta vectors from pixel to pixel
        _pixel_delta_u = {viewport_u / _image_width};
        _pixel_delta_v = {viewport_v / _image_height};

        // Calculate the location of the upper left pixel
        const Point3 viewport_upper_left{_centre - (_focus_dist * _w) -
                                         viewport_u / 2.0 - viewport_v / 2.0};
        _pixel00_loc = {viewport_upper_left +
                        0.5 * (_pixel_delta_u + _pixel_delta_v)};

        // Calculate the camera defocus disc basis vectors.
        const double defocus_radius{
            _focus_dist * tan(degrees_to_radians(_defocus_angle / 2.0))};
        _defocus_disc_u = _u * defocus_radius;
        _defocus_disc_v = _v * defocus_radius;
    }


    [[nodiscard]] Ray get_ray(int i, int j) const
    {
        // get a randomly sampled camera ray for the pixel at location i,j, originating from the camera defocus disc
        const Point3 pixel_centre{_pixel00_loc + (i * _pixel_delta_u) +
                                  (j * _pixel_delta_v)};
        const Point3 pixel_sample{pixel_centre + pixel_sample_square()};

        const Point3 ray_origin{(_defocus_angle <= 0) ? _centre
                                                      : defocus_disc_sample()};
        const Vec3 ray_direction{pixel_sample - ray_origin};

        return Ray{ray_origin, ray_direction};
    }

    [[nodiscard]] Vec3 pixel_sample_square() const
    {
        // returns a random point in the square surrounding a pixel at the origin
        const double px{-0.5 + random_double()};
        const double py{-0.5 + random_double()};
        return {(px * _pixel_delta_u) + (py * _pixel_delta_v)};
    }

    Point3 defocus_disc_sample() const
    {
        // Returns a random point in the camera defocus disc.
        Point3 point{random_in_unit_disc()};

        return _centre +
               (point[0] * _defocus_disc_u) * (point[1] * _defocus_disc_v);
    }

    [[nodiscard]] Colour ray_colour(const Ray &ray,
                                    int depth,
                                    const Hittable &world) const
    {
        // If we have exceeded the ray bounce limit, we stop gathering light
        if (depth <= 0)
        {
            return Colour{0.0, 0.0, 0.0};
        }

        HitRecord record;

        if (world.hit(ray, Interval(0.001, constants::kInfinity), record))
        {
            Ray scattered;
            Colour attenuation;
            if (record._material->scatter(ray, record, attenuation, scattered))
            {
                return attenuation * ray_colour(scattered, depth - 1, world);
            }
            return Colour{
                0.0,
                0.0,
                0.0,
            };
        }

        const Vec3 unit_direction{unit_vector(ray.direction())};
        // NOLINTNEXTLINE(readability-magic-numbers)
        auto a_value{0.5 * (unit_direction.y() + 1.0)};

        return (1.0 - a_value) * Colour{1.0, 1.0, 1.0} +
               // NOLINTNEXTLINE(readability-magic-numbers)
               a_value * Colour{0.5, 0.7, 1.0};
    }
};

#endif
