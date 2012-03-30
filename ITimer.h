// TODO: DOCUMENTATION
// TODO: Replace time unit of "int" with a typedef...
// A timer interface.

#ifndef _ITIMER_H_
#define _ITIMER_H_

class ITimer
{
  public:
  virtual ~ITimer()
  {
  }

  virtual void init() = 0;
  virtual void deinit() = 0;

  // This is the function that uses the system's timer
  //  to prevent wasting CPU cycles. It may not be precise
  //  enough for all purposes, and should mostly just be
  //  used internally, while the wait(int ms) function
  //  will guarantee 1-millisecond-precision if there isn't
  //  too much CPU strain.
  virtual void lazy_wait(int ms) = 0;
  
  void busy_wait(int ms)
  {
    int time = get_time();
    while( ms > 0 )
    {
      ms = ms - (get_time() - time);
    }
  }

  void wait(int ms)
  {
    int lazy_ms, busy_ms;

    lazy_ms = (ms / _precision) * _precision;
    busy_ms = ms - lazy_ms;

    lazy_wait(lazy_ms);
    busy_wait(busy_ms);
  }

  int current_time() const
  {
    return _current_time;
  }

  int precision() const
  {
    return _precision;
  }

  // Determine and return the minimum number of milliseconds lazy_wait
  //  can rest for. The "duration" passed in is the amount of time 
  //  we will attempt to delay with. The greater this number, the more
  //  accurate the precision reading will be.
  int calc_precision(int duration)
  {
    int start_time, passed_time, calls;
    passed_time = 0;
    calls = 0;

    start_time = get_time();
    do
    {
      lazy_wait(1);
      ++calls;
    }
    while( (get_time() - start_time) < duration );
    passed_time = get_time() - start_time;

    _precision = passed_time / calls 
               // Handle fractions of ms by rounding up:
               + (passed_time % calls > 0) ? 1 : 0;

    return _precision;
  }

  virtual int get_time() = 0;

  private:

  void tick()
  {
    _current_time = get_time();
  }

  int _current_time;
  int _precision;
};

#endif
