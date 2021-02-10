#include "params.h"

#define ABORT()

RenderParams::RenderParams(int argc, char **argv) {
  if (argc > 1) setInt("resolution", atoi(argv[1]));
  if (argc > 2) setInt("samples", atoi(argv[2]));
  return;
}

void RenderParams::setInt(const char *key, int value) { 
  data[key].i = value; 
}

void RenderParams::setDbl(const char *key, double value) { 
  data[key].d = value; 
}

void RenderParams::setStr(const char *key, const char *value) {
  data[key].s = value;
}

void RenderParams::assertExists(const char *key) {
  if (!exists(key)) { 
    fprintf(stderr, "ERROR: params[%s] not defined.\n", key);
    exit(1);
  }
}

int RenderParams::getInt(const char *key) {
  assertExists(key);
  return data[key].i;
}

double RenderParams::getDbl(const char *key) {
  assertExists(key);
  return data[key].d;
}

const char *RenderParams::getStr(const char *key) {
  assertExists(key);
  return data[key].s;
}

bool RenderParams::exists(const char *key) {
  return (data.find(key) != data.end());
}
