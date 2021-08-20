#pragma once

#include <chrono>
#include <string>

class Timer {
  std::string message = "Timer";
  std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
  std::chrono::duration<float, std::milli> timeElapsed;
  bool stopped = false;

public:
  Timer() { };
  Timer(std::string message);
  Timer(const char *format, ...);
  ~Timer();

  Timer& start();
  Timer& stop();
  
  void display();
  void stopAndDisplay();
};


