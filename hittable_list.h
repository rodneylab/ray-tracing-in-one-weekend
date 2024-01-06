#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
#include <vector>

class HittableList : public Hittable
{
public:
    std::vector<std::shared_ptr<Hittable>> _objects;

    HittableList() = default;

    explicit HittableList(std::shared_ptr<Hittable> object)
    {
        add(object);
    }

    void clear()
    {
        _objects.clear();
    }

    void add(std::shared_ptr<Hittable> object)
    {
        _objects.emplace_back(object);
    }

    bool hit(const Ray &ray, Interval ray_t, HitRecord &rec) const override
    {
        HitRecord temp_rec;
        bool hit_anything = false;
        double closest_so_far = ray_t._max;

        for (const auto &object : _objects)
        {
            if (object->hit(ray,
                            Interval(ray_t._min, closest_so_far),
                            temp_rec))
            {
                hit_anything = true;
                closest_so_far = temp_rec._t_interval;
                rec = temp_rec;
            }
        }

        return hit_anything;
    }
};

#endif
