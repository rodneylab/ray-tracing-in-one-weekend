#ifndef INTERVAL_H
#define INTERVAL_H

#include "utility.h"

class Interval
{
public:
    double _min;
    double _max;

    Interval() : _min(+constants::kInfinity), _max(-constants::kInfinity)
    {
    } // default interval is empty

    Interval(double min, double max) : _min(min), _max(max)
    {
    }

    bool contains(double x) const
    {
        return _min <= x && x <= _max;
    }

    bool surrounds(double x) const
    {
        return _min < x && x < _max;
    }

    double clamp(double x) const
    {
        if (x < _min)
        {
            return _min;
        }
        if (x > _max)
        {
            return _max;
        }
        return x;
    }

    static const Interval empty, universe;
};

const static Interval Empty(+constants::kInfinity, -constants::kInfinity);
const static Interval Universe(-constants::kInfinity, +constants::kInfinity);

#endif
