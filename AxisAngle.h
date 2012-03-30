#ifndef _AXISANGLE_H_
#define _AXISANGLE_H_

//class Quaternion;

#include "Vector.h"

// NOTE: AxisAngles automatically normalize themselves upon
//        creation, but may need to be normalized if modified.
class AxisAngle {
public:
    AxisAngle();
    AxisAngle(const AxisAngle& other);
    AxisAngle(const Vector& axis, double angle);
    //AxisAngle(const Quaternion& quat);
    
    Vector axis;
    double angle;
};

#endif
