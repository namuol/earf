#ifndef _RAY_H
#define _RAY_H

#include "Vector.h"

struct Ray
{
    Ray()
    {
    }

    Ray(const Point& pos, const Vector& norm, double refr=1.0)
    : pos(pos), norm(norm), refr(refr)
    {
    }

    Point pos;
    Vector norm;

    double refr; // For keeping track of the last refractive index the ray travelled through.
};

#endif
