#pragma once

#include <chrono>
#include <string>
#include <utility>

class Timer {
  std::string message = "Timer";
  std::chrono::steady_clock::time_point startTime;
  std::chrono::duration<float, std::milli> timeElapsed;

public:
  Timer();
  Timer(std::string message);
  Timer(const char *format, ...);

  Timer& start();
  Timer& stop();
  
  void display();
  void stopAndDisplay();
};

class ScopedTimer : public Timer {
  template <class... Args>
  ScopedTimer(Args&&... args) : Timer(std::forward(args)...) { start(); }
  ~ScopedTimer() { stopAndDisplay(); }
};

