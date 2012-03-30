#ifndef _UTIL_H
#define _UTIL_H

#include <limits>
#define D_INFINITY std::numeric_limits<double>::infinity()
#define D_EPSILON std::numeric_limits<double>::epsilon()

#define CLAMP(x, min, max) ((x)<(min)?(min):((x)>(max)?(max):(x)))

#define getpixel(screen, x, y) \
    (((Uint32*)(screen)->pixels)[(y)*((screen)->pitch/4) + (x)])

#define setpixelc(screen, x, y, c) \
    (((Uint32*)(screen)->pixels)[(y)*((screen)->pitch/4) + (x)]) = c

#define setpixel(screen, x, y, r,g,b) \
    (((Uint32*)(screen)->pixels)[(y)*((screen)->pitch/4) + (x)]) = SDL_MapRGB((screen)->format,(r),(g),(b))

#define setpixel2(screen, i, r, g, b) \
    (((Uint32*)(screen)->pixels)[(i)]) = SDL_MapRGB((screen)->format,(r),(g),(b))

#define addpixel(screen, x, y, r, g, b) \
    (((Uint32*)(screen)->pixels)[(y)*((screen)->pitch/4) + (x)]) += SDL_MapRGB((screen)->format,CLAMP((r),0,255),CLAMP((g),0,255),CLAMP((b),0,255));

#define RAND(max) (((double)rand()/(double)RAND_MAX)*(max))


typedef unsigned char u8;

#include <cmath>

const double PI = 4.0*atan(1.0);

#endif
