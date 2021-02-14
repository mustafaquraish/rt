#include "params.h"

#define ABORT()

RenderParams::RenderParams(int argc, char **argv) {
  if (argc > 1) setStr("scene", argv[1]);

  if (argc > 2) {
    int sx, sy;
    // Expect `WxH` resolution. If just one number, then square
    if (sscanf(argv[2], "%dx%d", &sx, &sy) == 1) sy = sx;
    setInt("width", sx);
    setInt("height", sy);
  }

  if (argc > 3) setInt("samples", atoi(argv[3]));
  if (argc > 4) setStr("output", argv[4]);
  if (argc > 5) setInt("frame", atoi(argv[5]));

  return;
}

void RenderParams::setInt(const std::string &key, int value) { 
  data[key].i = value; 
}

void RenderParams::setDbl(const std::string &key, double value) { 
  data[key].d = value; 
}

void RenderParams::setStr(const std::string &key, const char *value) {
  data[key].s = value;
}

void RenderParams::assertExists(const std::string &key) {
  if (!exists(key)) { 
    fprintf(stderr, "ERROR: params[%s] not defined.\n", key.c_str());
    exit(1);
  }
}

int RenderParams::getInt(const std::string &key) {
  assertExists(key);
  return data[key].i;
}

double RenderParams::getDbl(const std::string &key) {
  assertExists(key);
  return data[key].d;
}

const char *RenderParams::getStr(const std::string &key) {
  assertExists(key);
  return data[key].s;
}

bool RenderParams::exists(const std::string &key) {
  return (data.find(key) != data.end());
}
