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

#ifndef __PROGRESS_H__
#define __PROGRESS_H__

#include <string>
#include "stdio.h"

struct ProgressBar {
  ProgressBar(int totalWork) : ProgressBar(totalWork, "ProgressBar") {};

  ProgressBar(int totalWork, std::string message) 
    : total(totalWork), workRatio(100.0/totalWork), message(message) {};

  void update(int increment=1) { 
    done += increment;
    display(); 
  };

  ProgressBar& start() { display(); return *this; }

  void display() {
    printf("\r- %s progress: %.3f%%", message.c_str(), done * workRatio);
    fflush(stdout);
  }

  int total;
  int done = 0;
  double workRatio;
  std::string message;
};

#endif // __PROGRESS_H__
