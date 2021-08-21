/**
 * Quick one-header progress tracker. Usage:
 * 
 *  ProgressBar progress(100, "My Task");
 *  progress.start();
 *  for (int i = 0; i < 100; i++) {
 *    // ...
 *    progress.update();
 *  }
 *  
 */

#pragma once

#include <string>
#include <stdio.h>

struct ProgressBar {
  ProgressBar(int totalWork) : ProgressBar(totalWork, "ProgressBar") {};

  ProgressBar(int totalWork, std::string message, int displayInc) 
    : total(totalWork), workRatio(100.0/totalWork), message(message),
      displayIncrement(displayInc) {};

  ProgressBar(int totalWork, std::string message) 
    : total(totalWork), workRatio(100.0/totalWork), message(message) {}


  void update(int increment=1) { 
    done += increment;
    if (done % displayIncrement == 0) display(); 
  };

  ProgressBar& start() { display(); return *this; }

  void display() {
    printf("\r- %s progress: %.1f%%  ", message.c_str(), done * workRatio);
  }

  int total;
  int done = 0;
  int displayIncrement=1;
  float workRatio;
  std::string message;
};

