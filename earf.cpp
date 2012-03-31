#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "ITimer.h"
#include "POSIXTimer.h"
#include "Camera.h"

#define SCR_W 640
#define SCR_H 400

using namespace std;
bool running = true;

void quit() {
  SDL_Quit();
}

void event(SDL_Event* e) {
  if (e->type == SDL_QUIT) {
    running = false;
  }
}

int main(int ac, char** av) {

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
  _map = IMG_Load("heightmap.png");
  map = SDL_DisplayFormat(_map);

  cout << scr->format << endl;
  cout << map->format << endl;

  Camera* cam = new Camera(Vector(127,64,127), 45, SCR_W,SCR_H);


  POSIXTimer timer;
  timer.init();
  timer.calc_precision(100);

  int cyclesLeftOver = 0;
  int lastFrameTime = 0;
  int updateInterval = 1000/60;
  int maxCyclesPerFrame = 8;
  SDL_Event e;

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
      cam->eye(cam->eye() + Vector(0.5,0.0,0.5));
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
      int maxY = SCR_H;
      double cx,cy,cz;
      double h, ch;
      Uint32 c;
      Uint8 r,g,b;
      Ray ray = cam->getRayFromUV(x,0);
      ch = ray.pos.y;
      for (double d = 15; d < 256; d += 1) {
        cx = ray.pos.x + ray.norm.x * d;
        cz = ray.pos.z + ray.norm.z * d;
        if (cx < 0 || cz < 0) continue;
        //cout << cx << ":" << cz << endl;
        c = getpixel(map, (int)cx%map->w,(int)cz%map->h);         
        h = (double)r * 0.25;
        //cout << "h: " << h << endl;
        SDL_GetRGB(c, scr->format, &r, &g, &b);
        //int y = SCR_H/2 - (((h - ch) * (13.4/d)) / 256.0) * SCR_H;
        int y = ((h - ch) * 250) / d + SCR_H; 
        //cout << y << endl;
        if (y < 0) continue;
        if (y >= SCR_H) break;
        
        //if (y < maxY) {
          //for (int _y = maxY; _y > y; --_y)
            setpixelc(scr, x,y, c);
          //maxY = y;
        //}
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
