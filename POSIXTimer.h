// TODO: DOCUMENTATION
// A timer via POSIX usleep and gettimeofday.

#ifndef _POSIXTIMER_H_
#define _POSIXTIMER_H_

#include "ITimer.h"

class POSIXTimer : public ITimer
{
  public:
  virtual void init();
  virtual void deinit();
  
  virtual void lazy_wait(int ms);
  virtual int get_time();

  private:
  int time_at_init;
};
#endif
