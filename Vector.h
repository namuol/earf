#ifndef _VECTOR_H_
#define _VECTOR_H_
class AxisAngle;

class Vector {
public:
    Vector();
    Vector(const Vector& other);
    Vector(double x, double y, double z);

    ~Vector();
    
    Vector& operator=(const Vector& v);

    Vector& operator+=(const Vector& v);
    Vector& operator-=(const Vector& v);
    Vector& operator*=(double s);
    Vector& operator/=(double s);
    Vector operator+(const Vector& v) const;
    Vector operator-(const Vector& v) const;
    Vector operator*(double s) const;
    Vector operator/(double s) const;
    Vector operator-(void) const;

    double dot(const Vector& v) const;
    Vector cross(const Vector& v) const;
    Vector lerp(const Vector& v, double u) const;
    Vector rotate(const AxisAngle& a) const;
    
    double length() const;
    double lengthSquared() const;
    const Vector& normalize();

    Vector normal() const;

    double x, y, z;
};

// Points are conceptually different, but internally identical to a vector at the origin.
typedef Vector Point;

#endif
