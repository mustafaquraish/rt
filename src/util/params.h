#ifndef __RT_PARAMS_H__
#define __RT_PARAMS_H__

#include <string>
#include "core/math.h"
#include <unordered_map>

using std::string;

union RenderParamItem {
  int i;
  double d;
  const char *s;
  bool b;
};

struct RenderParams {
  RenderParams() {} ;
  RenderParams(int argc, char **argv);

  void update(int frame);
  void setWindow(double xBegin, double xEnd, double yBegin, double yEnd);

  template <typename T>
  void set(const std::string &key, T value);

  template <typename T>
  T get(const std::string &key);

  bool exists(const std::string &key);

private:
  std::unordered_map<std::string, RenderParamItem> data;
  void assertExists(const std::string &key);
};


#endif // __RT_PARAMS_H__
