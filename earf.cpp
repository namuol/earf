#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_keysym.h>
#include <SDL/SDL_image.h>

#include "ITimer.h"
#include "POSIXTimer.h"
#include "Camera.h"

#define SCR_W 640
#define SCR_H 400

using namespace std;
bool running = true;
bool leftdown, rightdown, updown, downdown;

void quit() {
  SDL_Quit();
}

void handle_key(SDL_Event* e, bool keydown) {
  switch (e->key.keysym.sym) {
    case SDLK_LEFT:
      leftdown = keydown;
      break;
    case SDLK_RIGHT:
      rightdown = keydown;
      break;
    case SDLK_UP:
      updown = keydown;
      break;
    case SDLK_DOWN:
      downdown = keydown;
      break;
    default:
      break;
  }
}

void event(SDL_Event* e) {
  switch (e->type) {
  case SDL_QUIT:
    running = false;
    break;
  case SDL_KEYDOWN:
    if (e->key.keysym.sym == SDLK_ESCAPE) {
      running = false;
      break;
    }
    handle_key(e, true);
    break;
  case SDL_KEYUP:
    handle_key(e, false);
    break;
  }
}

int main(int ac, char** av) {
  cout << -4 % 255 << endl;

  SDL_Surface* scr;

  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    quit();
    return -1;
  } 
  
  if ((scr = SDL_SetVideoMode(SCR_W,SCR_H,32, SDL_HWSURFACE|SDL_DOUBLEBUF)) == NULL) {
    quit();
    return -2;
  }
  SDL_Surface* _map, * map;
  _map = IMG_Load("heightmap.jpg");
  //_map = IMG_Load("wat.jpg");
  map = SDL_DisplayFormat(_map);

  cout << scr->format << endl;
  cout << map->format << endl;

  Camera* cam = new Camera(Vector(map->w/2,128,map->h/2), 25, SCR_W,SCR_H);


  POSIXTimer timer;
  timer.init();
  timer.calc_precision(100);

  int cyclesLeftOver = 0;
  int lastFrameTime = 0;
  int updateInterval = 1000/60;
  int maxCyclesPerFrame = 8;
  SDL_Event e;

  Vector cv;
  while( running )
  {
    int currentTime;
    int updateIterations;
    int dt;

    currentTime = timer.get_time();
    dt = (currentTime - lastFrameTime);
    updateIterations = (dt + cyclesLeftOver);

    if (updateIterations > (maxCyclesPerFrame * updateInterval)) {
      updateIterations = (maxCyclesPerFrame * updateInterval);
    }
    
    while (updateIterations > updateInterval) {
      updateIterations -= updateInterval;

      // UPDATE EVERYTHING.
      if (updown)
        cv.z -= 0.2;
      if (downdown)
        cv.z += 0.2;
      if (leftdown)
        cv.x -= 0.2;
      if (rightdown)
        cv.x += 0.2;
      

      double h; 
      Uint8 r,g,b;
      Vector eye = cam->eye();
      if (eye.x >= 0 && eye.z >= 0) {
        Uint32 c = getpixel(map, (int)(eye.x)%map->w,(int)(eye.z)%map->h);         
        SDL_GetRGB(c, scr->format, &r, &g, &b);
        h = (double)r * 0.25;
        double target = (h+55) - eye.y;
        cv.y = (target - cv.y) * 0.1;
      }
      cam->eye(cam->eye() + cv);
      cv *= 0.9;
      while (SDL_PollEvent(&e)) {
        event(&e);
      }
    }

    cyclesLeftOver = updateIterations;
    lastFrameTime = currentTime;

    // RENDER EVERYTHING.
    SDL_FillRect( SDL_GetVideoSurface(), NULL, 0 );
    if(SDL_MUSTLOCK(scr)) SDL_LockSurface(scr);
    if(SDL_MUSTLOCK(map)) SDL_LockSurface(map);
    for (int x=0; x < SCR_W; ++x) {
      double maxH = 0;
      int maxY = SCR_H-1;
      double cx,cy,cz;
      double h, ch;
      Uint32 c;
      Uint8 r,g,b;
      Ray ray = cam->getRayFromUV(x,0);
      ch = ray.pos.y;
      for (double d = 5; d < 255; d += 1) {
        cx = 1 * (ray.pos.x + ray.norm.x * d);
        cz = 1 * (ray.pos.z + ray.norm.z * d);
        if (cx < 0 || cz < 0) continue;
        //cout << cx << ":" << cz << endl;
        c = getpixel(map, (int)cx%map->w,(int)cz%map->h);         
        //c = getpixel(map, mod((int)cx,map->w),mod((int)cz,map->h));         
        //cout << "h: " << h << endl;
        SDL_GetRGB(c, scr->format, &r, &g, &b);
        h = (double)r * 0.25;
        /*
        if (h < 15) {
          h = 15;
          r = 10;
          g = 35;
          b = 50;
        }
        */

        //int y = SCR_H - (((h - ch) * (13.4/d)) / 256.0) * SCR_H;
        int y = SCR_H - (((h - ch) * 350) / d + SCR_H);

        //cout << y << endl;
        if (y < 0) break;
        if (y >= SCR_H) continue;

        double fog = 1.0 - d/255;
        r*=fog;
        g*=fog;
        b*=fog;
        if (y < maxY) {
          for (int _y = maxY; _y > y; --_y)
            setpixel(scr, x,_y, r,g,b);
          maxY = y;
        }
      }
    }
    // For each column of pixels...
    //   For each pixel in column (bottom-to-top)...
    //     Cast Ray for pixel:
    //       For each point in heightmap along this ray, starting at current highest...
    //         RETURN if off the map.
    //         Compute distance D along ray to point centered above heightmap point.
    //         Is height of ray at D below heightmap?
    //          YES: set current highest to current heightmap point
    //          NO:  move "forward" one point on heightmap.
    //     
    SDL_UnlockSurface(scr);
    SDL_UnlockSurface(map);
    SDL_Flip(scr);

    int leftoverTime = updateInterval - (timer.get_time() - lastFrameTime);
    if (leftoverTime < 0)
      cout << leftoverTime << endl;
    // If we have more than 1 update interval worth of time left still,
    //  wait it out to save CPU cycles.
    if( leftoverTime > timer.precision() )
    {
      timer.wait(leftoverTime - (leftoverTime % timer.precision()));
    }

    while( leftoverTime > 0 )
    {
      leftoverTime = updateInterval - (timer.get_time() - lastFrameTime);
    }
  }

  quit();
  return 0;
}
