#include "camera.h"
#include "colour.h"
#include "hittable.h"
#include "hittable_list.h"
#include "ray.h"
#include "sphere.h"
#include "utility.h"
#include "vec3.h"

#include <iostream>
#include <memory>

int main()
{
    HittableList world;

    // NOLINTBEGIN(readability-magic-numbers)
    auto ground_material(std::make_shared<Lambertian>(Colour{0.5, 0.5, 0.5}));
    world.add(
        std::make_shared<Sphere>(Point3{0, -1000, 0}, 1000, ground_material));

    for (int aa{-11}; aa < 11; ++aa)
    {
        for (int bb{-11}; bb < 11; ++bb)
        {
            const double choose_mat{random_double()};
            Point3 centre{aa + 0.9 * random_double(),
                          0.2,
                          bb + 0.9 * random_double()};

            if ((centre - Point3{4, 0.2, 0}).length() > 0.9)
            {
                std::shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8)
                {
                    // diffuse
                    const Colour albedo{Colour::random() * Colour::random()};
                    sphere_material = std::make_shared<Lambertian>(albedo);
                    world.add(
                        std::make_shared<Sphere>(centre, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95)
                {
                    // metal
                    const Colour albedo{Colour::random(0.5, 1)};
                    auto fuzz(random_double(0, 0.5));
                    sphere_material = std::make_shared<Metal>(albedo, fuzz);
                    world.add(
                        std::make_shared<Sphere>(centre, 0.2, sphere_material));
                }
                else
                {
                    //glass
                    sphere_material = std::make_shared<Dielectric>(1.5);
                    world.add(
                        std::make_shared<Sphere>(centre, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1(std::make_shared<Dielectric>(1.5));
    world.add(std::make_shared<Sphere>(Point3{0, 1, 0}, 1.0, material1));

    auto material2(std::make_shared<Lambertian>(Colour{0.4, 0.2, 0.1}));
    world.add(std::make_shared<Sphere>(Point3{-4, 1, 0}, 1.0, material2));

    auto material3(std::make_shared<Metal>(Colour{0.7, 0.6, 0.5}, 0.0));
    world.add(std::make_shared<Sphere>(Point3{4, 1, 0}, 1.0, material3));

    Camera camera;
    camera._aspect_ratio = 16.0 / 9.0;
    camera._image_width = 1200;
    camera._samples_per_pixel = 500;
    camera._max_depth = 50;
    //camera._samples_per_pixel = 10;
    //camera._max_depth = 20;

    camera._vertical_fov = 20;
    camera._look_from = Point3{13, 2, 3};
    camera._look_at = Point3{0, 0, 0};
    camera._vup = Point3{0, 1, 0};

    camera._defocus_angle = 0.6;
    camera._focus_dist = 10.0;

    // NOLINTEND(readability-magic-numbers)

    camera.render(world);
}
