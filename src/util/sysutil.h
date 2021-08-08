#pragma once

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

/** Run command on system shell **/
int run_command(const char *format, ...) {
  char buffer[512];
  va_list args;
  va_start(args, format);
  vsprintf(buffer, format, args);
  va_end(args);
  int ret = system(buffer);
  if (ret != 0)
    fprintf(stderr, "Error with system(%s)\n", buffer);
  return ret;
}

