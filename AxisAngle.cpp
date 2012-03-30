#include <cmath>

//#include "Quaternion.h"
#include "Vector.h"
#include "AxisAngle.h"

AxisAngle::AxisAngle(const AxisAngle& other)
: axis(other.axis), angle(other.angle)
{
    axis.normalize();
}

AxisAngle::AxisAngle(const Vector& axis, double angle)
: axis(axis), angle(angle)
{
    this->axis.normalize();
}

/*
AxisAngle::AxisAngle(const Quaternion& quat)
: angle(2.0f*acos(quat.w()))
{
    axis = quat.v() / sin(angle/2.0f);
    axis.normalize();
}
*/

AxisAngle::AxisAngle()
: axis(Vector(1,0,0)), angle(0.0f)
{
    axis.normalize();
}
