#include <cmath>
#include <cstdio>
#include <iostream>
#include <limits>

using namespace std;

#include "AxisAngle.h"
#include "Vector.h"

Vector::Vector()
: x(0), y(0), z(0)
{
}

Vector::Vector(const Vector& v)
//: x(v.x), y(v.y), z(v.z)
{
    this->x = v.x;
    this->y = v.y;
    this->z = v.z;
}

Vector::Vector(double x, double y, double z)
: x(x), y(y), z(z)
{
}

Vector::~Vector()
{
}

double Vector::length() const
{
    return sqrt(x*x + y*y + z*z);
}

double Vector::lengthSquared() const
{
    return x*x + y*y + z*z;
}


Vector Vector::normal() const 
{ 
    double mag = sqrt(x*x + y*y + z*z);
    if( abs(mag) < numeric_limits<double>::epsilon() ) {
        return Vector(0.0,0.0,0.0);
    }
    return Vector(x/mag, y/mag, z/mag); 
}

const Vector& Vector::normalize() 
{ 
    double mag = sqrt(x*x + y*y + z*z);
    if( abs(mag) < numeric_limits<double>::epsilon() ) {
        x = 0.0;
        y = 0.0;
        z = 0.0;
    } else {
        x /= mag;
        y /= mag;
        z /= mag;
    }
    return *this;
}

Vector& Vector::operator=(const Vector& v)
{
    if(this == &v)
        return *this;

    x = v.x;
    y = v.y;
    z = v.z;

    return *this; 
}

Vector& Vector::operator+=(const Vector& v)
{
    *this = *this + v;
    return *this;
}

Vector& Vector::operator-=(const Vector& v)
{
    *this = *this - v;
    return *this;
}

Vector& Vector::operator*=(double s)
{
    x *= s;
    y *= s;
    z *= s;
    return *this;
}

Vector& Vector::operator/=(double s)
{
    x /= s;
    y /= s;
    z /= s;
    return *this;
}

Vector Vector::operator+(const Vector& v) const
{
    return Vector(x+v.x, y+v.y, z+v.z);
}

Vector Vector::operator-(const Vector& v) const
{
    return Vector(x-v.x, y-v.y, z-v.z);
}

Vector Vector::operator*(double scalar) const
{
    return Vector(x*scalar, y*scalar, z*scalar);
}

Vector Vector::operator/(double scalar) const
{
    return Vector(x/scalar, y/scalar, z/scalar);
}

Vector Vector::operator-(void) const
{
    return Vector(-x, -y, -z);
}

double Vector::dot(const Vector& v) const
{
    return x*v.x + y*v.y + z*v.z;
}

Vector Vector::cross(const Vector& v) const
{
    return Vector( y*v.z - z*v.y,
                   z*v.x - x*v.z,
                   x*v.y - y*v.x );
}

Vector Vector::lerp(const Vector& v, double u) const
{
    return  *this + ((v - *this) * u); 
}

