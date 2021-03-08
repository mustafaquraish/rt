#ifndef __SYSUTIL_H__
#define __SYSUTIL_H__

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

/** Run command on system shell **/
int runCommand(const char *format, ...) {
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

#endif // __SYSUTIL_H__
