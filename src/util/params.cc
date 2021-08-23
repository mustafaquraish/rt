#include <util/params.h>
#include <vector>

void print_usage() {
  printf("  Usage:                                                           \n");
  printf("  rt (scene) [...OPTIONS]                                          \n");
  printf("                                                                   \n");
  printf("  -s, --samples arg     Samples per pixel (default: 10)            \n");
  printf("  -o, --output arg      Output file (ppm) (default: output.ppm)    \n");
  printf("  -a, --animate         Render animation                           \n");
  printf("  -f, --frame arg       Frame number(s) A[:B][/Total] (default: 0) \n");
  printf("  -r, --resolution arg  Resolution A[xB] (default: 512x512)        \n");
  printf("  -t, --tile-size arg   Size of each tile (default: 8)             \n");
  printf("  -h, --help            Print Usage                                \n"); 
  exit(0);
}

template <>
void RenderParams::set<int>(const std::string &key, int value) { 
  data[key].i = value; 
}

template <>
void RenderParams::set<bool>(const std::string &key, bool value) { 
  data[key].b = value; 
}

template <>
void RenderParams::set<float>(const std::string &key, float value) { 
  data[key].d = value; 
}

template <>
void RenderParams::set<const char *>(const std::string &key, const char *val) {
  data[key].s = val;
}

template <>
void RenderParams::set<char *>(const std::string &key, char *val) {
  data[key].s = val;
}

void RenderParams::assertExists(const std::string &key) {
  if (!exists(key)) { 
    fprintf(stderr, "ERROR: params[%s] not defined.\n", key.c_str());
    exit(1);
  }
}

template <>
int RenderParams::get<int>(const std::string &key) {
  assertExists(key);
  return data[key].i;
}

template <>
bool RenderParams::get<bool>(const std::string &key) {
  assertExists(key);
  return data[key].b;
}

template <>
float RenderParams::get<float>(const std::string &key) {
  assertExists(key);
  return data[key].d;
}

template <>
const char *RenderParams::get<const char *>(const std::string &key) {
  assertExists(key);
  return data[key].s;
}

template <>
char *RenderParams::get<char *>(const std::string &key) {
  assertExists(key);
  return (char *)data[key].s;
}

bool RenderParams::exists(const std::string &key) {
  return (data.find(key) != data.end());
}

void RenderParams::update_frame(int frame) {
  // Init stuff for static image
  set("frame", frame);
  float frameRatio = frame / (float) get<int>("frameTotal");
  set("frameRatio", frameRatio);
}

void RenderParams::set_window(float xBegin, float xEnd, 
                              float yBegin, float yEnd) {
  set("xBegin", xBegin);
  set("yBegin", yBegin);
  set("xEnd", xEnd);
  set("yEnd", yEnd);
}


RenderParams::RenderParams(int argc, char **argv) {

  std::vector<std::string> args(argv, argv + argc);

  int samples = 10;
  int xRes = 512;
  int yRes = 512;
  const char *output = "output.ppm";
  const char *scene = "Cornell";
  
  bool animate = false;
  int frameBegin = 0;
  int frameEnd = 1;
  int frameCount = 1;
  int frameTotal = 60;
  int tileSize = 8;


  // Loop over command-line args
  // (Actually I usually use an ordinary integer loop variable and compare
  // args[i] instead of *i -- don't tell anyone! ;)
  int positional = 0;
  for (int i = 1; i < argc; i++) {
    if (args[i] == "-h" || args[i] == "--help") {
      print_usage();
    } else if (args[i] == "-s" || args[i] == "--samples") {
      samples = stoi(args[++i]);
    } else if (args[i] == "-t" || args[i] == "--tile-size") {
      tileSize = stoi(args[++i]);
    } else if (args[i] == "-o" || args[i] == "--output") {
      output = argv[++i];
    } else if (args[i] == "-a" || args[i] == "--animate") {
      animate = true;
    } else if (args[i] == "-f" || args[i] == "--frame") {
      const char *ss = argv[++i];
      if (sscanf(ss, "%d:%d/%d", &frameBegin, &frameEnd, &frameTotal) == 3) {
        frameCount = frameEnd-frameBegin;
      } else if (sscanf(ss, "%d+%d/%d", &frameBegin, &frameCount, &frameTotal) == 3) {
        frameEnd = frameBegin + frameCount;
      } else if (sscanf(ss, "%d:%d", &frameBegin, &frameCount) == 2) {
        frameCount = frameEnd-frameBegin;
      } else if (sscanf(ss, "%d+%d", &frameBegin, &frameCount) == 2) {
        frameEnd = frameBegin + frameCount;
      } else if (sscanf(ss, "%d/%d", &frameBegin, &frameTotal) == 2) {
        frameEnd = frameBegin + 1;
      } else if (sscanf(ss, "%d", &frameBegin) == 1) {
        frameEnd = frameBegin + 1;
      } else {
        printf("Invalid frame selection `%s`\n", ss);
        exit(0);
      }
    } else if (args[i] == "-r" || args[i] == "--resolution") {
      const char *ss = argv[++i];
      if (sscanf(ss, "%dx%d", &xRes, &yRes) == 1) { yRes = xRes; } 
    } else {
      switch (positional++) {
        case 0: scene = argv[i]; break;
        default: printf("Unrecognized argument: '%s'\n", argv[i]); break;
      }
    }
  }

#if 0
  printf("- Param: samples = %d\n", samples);
  printf("- Param: width = %d\n", xRes);
  printf("- Param: height = %d\n", yRes);
  printf("- Param: animate = %d\n", animate);
  printf("- Param: frameBegin = %d\n", frameBegin);
  printf("- Param: frameEnd = %d\n", frameEnd);
  printf("- Param: frameCount = %d\n", frameCount);
  printf("- Param: frameTotal = %d\n", frameTotal);
  printf("- Param: output = %s\n", output);
  printf("- Param: scene = %s\n", scene);
#endif

  set("samples", samples);
  set("width", xRes);
  set("height", yRes);
  set("animate", animate);
  set("frameBegin", frameBegin);
  set("frameEnd", frameEnd);
  set("frameCount", frameCount);
  set("frameTotal", frameTotal);
  set("output", output);
  set("scene", scene);
  set("tileSize", tileSize);

  set_window(0, 1, 0, 1);
  update_frame(frameBegin);
}