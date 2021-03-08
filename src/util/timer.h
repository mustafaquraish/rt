#ifndef __TIMER_H__
#define __TIMER_H__

#include <chrono>
#include <string>

class Timer {
  std::string message = "Timer";
  std::chrono::steady_clock::time_point startTime;
  std::chrono::duration<double, std::milli> timeElapsed;

public:
  Timer();
  Timer(std::string message);
  Timer(const char *format, ...);

  Timer& start();
  Timer& stop();
  
  void display();
  void stopAndDisplay();
};

#endif