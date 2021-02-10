#ifndef __RT_PARAMS_H__
#define __RT_PARAMS_H__

#include <unordered_map>

union RenderParamItem {
  int i;
  double d;
  const char *s;
};

struct RenderParams {
  RenderParams() {} ;
  RenderParams(int argc, char **argv);

  void setInt(const char *key, int    value);
  void setDbl(const char *key, double value);
  void setStr(const char *key, const char * value);

  int          getInt(const char *key);
  double       getDbl(const char *key);
  const char * getStr(const char *key);

  bool exists(const char *key);

private:
  std::unordered_map<const char *, RenderParamItem> data;
  void assertExists(const char *key);
};


#endif // __RT_PARAMS_H__
