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
    }

    Camera(const Camera& other)
    : _eye(other.eye()), _scr_w(other.scr_w()), _scr_h(other.scr_h()) 
    {
        fovy(other.fovy());
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

    // Returns a Vector which points in the direction of an eye ray for
    //  a given (sub)pixel coordinate:
    /*
    Ray getRayFromUV(double u, double v) const
    {
        return Ray(_eye, Vector( _xstart + u*_xmult, _ystart + v*_ymult, -1 ));
    }
    */

    Ray getRayFromUV(int u, int v) const
    {
        return Ray(_eye, Vector( _xstart + u*_xmult, _ystart + v*_ymult, -1 ).normal());
    }

    void fastCast(int u, int v, double d, double& x, double& y, double& z) const
    {
      x = _eye.x + (_xstart + u*_xmult) * d;
      y = _eye.y + (_ystart + v*_ymult) * d;
      z = _eye.z + -d; 
    }

private:
    Point _eye;
    double _fovx,_fovy,
           _xstart,_ystart,
           _xmult,_ymult;

    int _scr_w, _scr_h;
};

#endif
