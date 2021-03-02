#include "util/timer.h"
#include <iostream>

Timer::Timer(){}

Timer::Timer(std::string message) : message(message) { }

using namespace std::chrono;

Timer& Timer::start() {
  startTime = std::chrono::steady_clock::now();
  return *this;
}

Timer& Timer::stop() {
  auto endTime = std::chrono::steady_clock::now();
  timeElapsed += endTime - startTime;
  return *this;
}

void Timer::display() {
  auto mins = duration_cast<minutes>(timeElapsed).count();
  auto secs = duration_cast<seconds>(timeElapsed).count() - mins * 60;
  auto mill = duration_cast<milliseconds>(timeElapsed).count() - secs * 1000;
  std::cout << "\r[+] " << message << " completed in: " 
                        << mins << "m" 
                        << secs << "."
                        << mill << "s" << std::endl;
}

void Timer::stopAndDisplay() {
  stop();
  display();
}