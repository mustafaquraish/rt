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
  int mins = duration_cast<minutes>(timeElapsed).count();
  int secs = duration_cast<seconds>(timeElapsed).count() - mins * 60;
  int mill = duration_cast<milliseconds>(timeElapsed).count() - secs * 1000;
  printf("\r[+] %s completed in: %dm%d.%03ds\n",
         message.c_str(), mins, secs, mill);
}

void Timer::stopAndDisplay() {
  stop();
  display();
}