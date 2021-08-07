#include <util/image/ppm.h>

namespace PPM {

template <typename T> 
static T swapEndian(T u) {
  static_assert(CHAR_BIT == 8, "CHAR_BIT != 8");

  union {
    T u;
    unsigned char u8[sizeof(T)];
  } source, dest;

  source.u = u;

  for (size_t k = 0; k < sizeof(T); k++)
    dest.u8[k] = source.u8[sizeof(T) - k - 1];

  return dest.u;
}

template <typename T>
void readImageData(FILE *f, float *arr, size_t num_items, float scale) {
  T *raw = new T[num_items];
  if (!fread(raw, sizeof(T), num_items, f))
    fprintf(stderr, "Error reading image\n");

  for (int i = 0; i < num_items; ++i)
    raw[i] = swapEndian<T>(raw[i]);

  for (int i = 0; i < num_items; ++i)
    arr[i] = raw[i] / scale;
  delete[] raw;
}

bool load(Image &img, const char *filename) {
  char line[1024];

  FILE *f = fopen(filename, "rb+");
  if (f == NULL) {
    std::cerr << "Error: Unable to open file " << filename << " for reading\n";
    return false;
  }

  (void) fgets(line, 1000, f);
  if (strcmp(line, "P6\n") != 0) {
    std::cerr << "Error: Wrong file format.\n";
    return false;
  }
  // Skip over comments
  do { (void) fgets(line, 511, f); } while (line[0] == '#');
  sscanf(&line[0], "%d %d\n", &img.sx, &img.sy); // Read file size

  (void) fgets(&line[0], 9, f);           // Read the remaining header line
  int maxRGB;
  sscanf(&line[0], "%d\n", &maxRGB); // Read file size

  img.m_data = new float[img.sx * img.sy * 3];

  // printf("===maxRGB is %d\n", maxRGB);
  if (maxRGB == 255) {
    readImageData<uint8_t>(f, img.m_data, 3 * img.sx * img.sy, maxRGB);
  } else {
    readImageData<uint16_t>(f, img.m_data, 3 * img.sx * img.sy, maxRGB);
  }

  fclose(f);
  return true;
}

bool save(Image &img, const char *filename) {
  FILE *f;
  unsigned char *bits24 = new unsigned char[img.sx * img.sy * 3];
  for (int i = 0; i < img.sx * img.sy * 3; i++) {
    bits24[i] = img.m_data[i] * 255.0;
  }

  f = fopen(filename, "wb+");
  if (f == NULL) {
    std::cerr << "Unable to open file " << filename << "\n";
    return false;
  }

  fprintf(f, "P6\n");
  fprintf(f, "%d %d\n", img.sx, img.sy);
  fprintf(f, "255\n");
  fwrite(bits24, img.sx * img.sy * 3 * sizeof(unsigned char), 1, f);
  fclose(f);
  delete[] bits24;
  return true;
}

} // namespace PPM