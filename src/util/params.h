#pragma once

#include <string>
#include <core/math.h>
#include <unordered_map>

using std::string;

union RenderParamItem {
  int i;
  float d;
  const char *s;
  bool b;
};

struct RenderParams {
  RenderParams() {} ;
  RenderParams(int argc, char **argv);

  void update_frame(int frame);
  void set_window(float xBegin, float xEnd, float yBegin, float yEnd);

  template <typename T>
  void set(const std::string &key, T value);

  template <typename T>
  T get(const std::string &key);

  bool exists(const std::string &key);

private:
  std::unordered_map<std::string, RenderParamItem> data;
  void assertExists(const std::string &key);
};


