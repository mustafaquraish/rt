/**
 * Stolen shamelessly from Silviu :)
 *  https://github.com/SilviuDraghici/Render3D/blob/master/src/utils/timer.h
 */
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
  
  Timer& start();
  Timer& stop();
  
  void display();
  void stopAndDisplay();
};

#endif