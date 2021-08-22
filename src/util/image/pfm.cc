#include <util/image/pfm.h>

namespace PFM {

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

bool load(Image &img, const char *filename) {
  char line[1024];

  FILE *f = fopen(filename, "rb+");
  if (f == NULL) {
    std::cerr << "Error: Unable to open file " << filename << " for reading\n";
    return false;
  }

  assert(fgets(line, 1000, f));
  if (strcmp(line, "PF\n") != 0) {
    std::cerr << "Error: Wrong file format.\n";
    return false;
  }
  // Skip over comments
  do { assert(fgets(line, 511, f)); } while (line[0] == '#');
  sscanf(&line[0], "%d %d\n", &img.sx, &img.sy); // Read file size

  assert(fgets(&line[0], 9, f));           // Read the remaining header line
  float endianness_scale;
  sscanf(&line[0], "%f\n", &endianness_scale); // Read file size

  float *buffer = new float[img.sx * img.sy * 3];
  fread(buffer, sizeof(float), img.sx * img.sy * 3, f);

  // This means we have big endian, convert to little endian for x86
  if (endianness_scale > 0) {
    for (int i = 0; i < img.sx * img.sy * 3; i++) {
      buffer[i] = swapEndian(buffer[i]);
    }
  }

  img.m_data = new float[img.sx * img.sy * 3];

  size_t num_vals = img.sx * img.sy * 3;
  for (size_t y = 0; y < img.sy; y++) {
    for (size_t x = 0; x < img.sx; x++) {
      size_t i_inp = (x + y * img.sx) * 3;
      size_t i_out = (x + (img.sy - y - 1) * img.sx) * 3;
      img.m_data[i_out + 0] = buffer[i_inp + 0];
      img.m_data[i_out + 1] = buffer[i_inp + 1];
      img.m_data[i_out + 2] = buffer[i_inp + 2];
    }
  }
  
  delete[] buffer;
  fclose(f);
  return true;
}

bool save(Image &img, const char *filename) {
  FILE *f;
  float *output = new float[img.sx * img.sy * 3];
  
  size_t num_vals = img.sx * img.sy * 3;
  for (size_t y = 0; y < img.sy; y++) {
    for (size_t x = 0; x < img.sx; x++) {
      size_t i_inp = (x + y * img.sx) * 3;
      size_t i_out = (x + (img.sy - y - 1) * img.sx) * 3;
      output[i_out + 0] = img.m_data[i_inp + 0];
      output[i_out + 1] = img.m_data[i_inp + 1];
      output[i_out + 2] = img.m_data[i_inp + 2];
    }
  }

  f = fopen(filename, "wb+");
  if (f == NULL) {
    std::cerr << "Unable to open file " << filename << "\n";
    return false;
  }

  fprintf(f, "PF\n");
  fprintf(f, "%d %d\n", img.sx, img.sy);
  fprintf(f, "-1.0\n");
  fwrite(output, img.sx * img.sy * 3 * sizeof(float), 1, f);
  fclose(f);
  delete[] output;
  return true;
}

};