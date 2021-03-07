#include "util/image.h"


void RTImageList::registerImage(std::string filename, Image *img) {
  imgFileMapping[filename] = img;
}

Image *RTImageList::getImage(std::string filename) {
  if (imgFileMapping.find(filename) == imgFileMapping.end())
    return NULL;
  return imgFileMapping[filename];
}

void RTImageList::cleanup() {
  for (auto it : imgFileMapping)
    delete it.second;
  imgFileMapping.clear();
}

/******************************************************************************/

Image::Image(int sx, int sy) : sx(sx), sy(sy) {
  data = new double[sx * sy * 3];
}

// The following functions for loading / saving PPM images are taken
// from an image processing library created by Francisco J Estrada.
// His academic page can be found at: http://www.cs.utoronto.ca/~strider/
// Modified to work with this Ray-Tracer

Image::Image(char const *fname) {
  FILE *f;
  char line[1024];

  char *tmp; // | These are to keep the compiler happy. Want to ignore
  int un;    // | Value of fgets() and fread()

  f = fopen(fname, "rb+");
  if (f == NULL) {
    std::cerr << "Error: Unable to open file " << fname << " for reading\n";
    exit(1);
  }

  tmp = fgets(line, 1000, f);
  if (strcmp(line, "P6\n") != 0) {
    std::cerr << "Error: Wrong file format.\n";
    exit(1);
  }

  // Skip over comments
  do {
    tmp = fgets(line, 511, f);
  } while (line[0] == '#');
  sscanf(&line[0], "%d %d\n", &sx, &sy); // Read file size
  tmp = fgets(&line[0], 9, f);           // Read the remaining header line

  unsigned char *raw = new unsigned char[sx * sy * 3];
  // Read the data
  un = fread(raw, sx * sy * 3, sizeof(unsigned char), f);

  data = new double[sx * sy * 3];
  for (int i = 0; i < sx * sy * 3; i++)
    data[i] = raw[i] / 255.0;

  un = un || tmp; // Use both temp variables so compiler doesn't shout
  delete[] raw;
  fclose(f);
}

Vec Image::get(int i, int j) {
  return Vec(
    data[(i + j * sx)*3 + 0],
    data[(i + j * sx)*3 + 1],
    data[(i + j * sx)*3 + 2]
  );
}

Vec Image::get(double u, double v) {
  double dx = u * (sx-1), dy = v * (sy-1);
  int f_x = dx, c_x = min(ceil(dx), sx-1);
  int f_y = dy, c_y = min(ceil(dy), sy-1);
  
  return bilerp(dx - f_x, dy - f_y, get(f_x, f_y), get(c_x, f_y),
                                    get(f_x, c_y), get(c_x, c_y));
}

double linearToSRGB(double L) {
  double S = L * 12.92;
  if (L > 0.0031308) {
    S = 1.055 * pow(L, 1.0 / 2.4) - 0.055;
  }
  return S;
}

void Image::save(char const *fname, bool gammaCorrect) {
  FILE *f;
  unsigned char *bits24 = new unsigned char[sx * sy * 3];
  for (int i = 0; i < sx * sy * 3; i++) {
    if (!gammaCorrect) {
      bits24[i] = data[i] * 255.0;
    } else {
      bits24[i] = linearToSRGB(data[i]) * 255.0;
    }
  }

  f = fopen(fname, "wb+");
  if (f == NULL) {
    std::cerr << "Unable to open file " << fname << "\n";
    return;
  }

  fprintf(f, "P6\n");
  fprintf(f, "%d %d\n", sx, sy);
  fprintf(f, "255\n");
  fwrite(bits24, sx * sy * 3 * sizeof(unsigned char), 1, f);
  fclose(f);
  delete[] bits24;
  printf("[+] Saved output file: %s\n", fname);
  return;
}

void Image::set(int i, int j, Colour col) {
  Colour clamped = clamp01(col);
  data[(i + j * sx) * 3 + 0] = clamped.x;
  data[(i + j * sx) * 3 + 1] = clamped.y;
  data[(i + j * sx) * 3 + 2] = clamped.z;
}

void Image::splat(int i, int j, Colour col) {
  data[(i + j * sx) * 3 + 0] = clamp01(data[(i + j * sx) * 3 + 0] + col.x);
  data[(i + j * sx) * 3 + 1] = clamp01(data[(i + j * sx) * 3 + 0] + col.y);
  data[(i + j * sx) * 3 + 2] = clamp01(data[(i + j * sx) * 3 + 0] + col.z);
}

Image::~Image() { 
  delete[] data; 
}