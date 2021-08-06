#include <util/timer.h>
#include <stdarg.h>
#include <string.h>

Timer::Timer(){}

Timer::Timer(std::string message) : message(message) { }

Timer::Timer(const char *format, ...) {
  char buffer[512];
  va_list args;
  va_start(args, format);
  vsprintf(buffer, format, args);
  va_end(args);
  message = std::string(buffer);
}

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