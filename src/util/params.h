#ifndef __RT_PARAMS_H__
#define __RT_PARAMS_H__

#include <string>
#include <unordered_map>

union RenderParamItem {
  int i;
  double d;
  const char *s;
};

struct RenderParams {
  RenderParams() {} ;
  RenderParams(int argc, char **argv);

  void setInt(const std::string &key, int    value);
  void setDbl(const std::string &key, double value);
  void setStr(const std::string &key, const char * value);

  int          getInt(const std::string &key);
  double       getDbl(const std::string &key);
  const char * getStr(const std::string &key);

  bool exists(const std::string &key);

private:
  std::unordered_map<std::string, RenderParamItem> data;
  void assertExists(const std::string &key);
};


#endif // __RT_PARAMS_H__
