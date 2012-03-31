#ifndef _CAMERA_H_
#define _CAMERA_H_
#include <iostream>
#include <cmath>

#include "util.h"

using namespace std;

#include "Vector.h"
#include "Ray.h"

class Camera {
public:
    Camera(const Vector& eye, double fovy, int scr_w, int scr_h )
    : _eye(eye), _scr_w(scr_w), _scr_h(scr_h) 
    {
        this->fovy(fovy);
        ang(0);
    }

    Camera(const Camera& other)
    : _eye(other.eye()), _scr_w(other.scr_w()), _scr_h(other.scr_h()) 
    {
        fovy(other.fovy());
        ang(0);
    }

    ~Camera()
    {
    }
    
    const Vector& eye() const { return _eye; }
    void eye( const Point& v ) { _eye = v; }

    double fovx() const { return _fovx; }

    double fovy() const { return _fovy; }
    void fovy( double v )
    {
        _fovy = v;
        _fovx = ((double)_scr_w / (double)_scr_h) * _fovy;

        // Precompute ray position variables for quick ray generation in "getRayFromUV":
        _xstart = -0.5*_fovx/45.0;
        _ystart = 0.5*_fovy/45.0;
        _xmult = (_fovx/45.0) / (double)_scr_w;
        _ymult = -(_fovy/45.0) / (double)_scr_h;
    }
    
    int scr_w() const { return _scr_w; }
    void scr_w( int v ) { _scr_w = v; }

    int scr_h() const { return _scr_h; }
    void scr_h( int v ) { _scr_h = v; }
    double ang() {return _ang;};
    void ang(double v) {
      _ang = v;
      _look = Vector(-sin(_ang), 0, -cos(_ang)).normal();
      _perp = Vector(-sin(_ang + PI/2), 0, -cos(_ang + PI/2)).normal();
    }

    // Returns a Vector which points in the direction of an eye ray for
    //  a given pixel coordinate:
    Ray getRayFromUV(int u, int v) const
    {
      Vector p = _look - (_perp * (_xstart + u*_xmult));
      return Ray(_eye, Vector(p.x, _ystart + v*_ymult, p.z).normal());
    }

    void fastCast(int u, int v, double d, double& x, double& y, double& z) const
    {
      x = _eye.x + (_xstart + u*_xmult) * d;
      y = _eye.y + (_ystart + v*_ymult) * d;
      z = _eye.z + -d; 
    }

    const Vector& look() { return _look; }
    const Vector& perp() { return _perp; }

private:
    Point _eye;
    double _fovx,_fovy,
           _xstart,_ystart,
           _xmult,_ymult;
    double _ang;
    Vector _look, _perp;

    int _scr_w, _scr_h;
};

#endif
