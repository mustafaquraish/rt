#include "util/image.h"

Image::Image(int sx, int sy) : sx(sx), sy(sy) {
  data = new double[sx * sy * 3];
  weights = new double[sx * sy];
  for (int i = 0; i < sx * sy; i++) 
    weights[i] = 1.0;
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

  printf("Loaded image with res: %d x %d\n", sx, sy);

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
  int f_x = dx, c_x = ceil(dx);
  int f_y = dy, c_y = ceil(dy);
  
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

void Image::set(int i, int j, const Colour &col) {
  Colour clamped = clamp01(col);
  data[(i + j * sx) * 3 + 0] = clamped.x;
  data[(i + j * sx) * 3 + 1] = clamped.y;
  data[(i + j * sx) * 3 + 2] = clamped.z;
}

void Image::splat(int i, int j, const Colour &col) {
  Colour clamped = clamp01(col);
  double wt = weights[i + j * sx];
  data[(i + j * sx) * 3 + 0] += clamped.x * pow(2, -log(wt));
  data[(i + j * sx) * 3 + 1] += clamped.y * pow(2, -log(wt));
  data[(i + j * sx) * 3 + 2] += clamped.z * pow(2, -log(wt));
  weights[i + j * sx] += col.x + col.y + col.z;
}

Image::~Image() { 
  delete[] data; 
  delete[] weights;
}


/**** HDR Save ***/

void Image::saveHDR(char const *fname) {
  FILE *f;
  double *imT;
  double HDRhist[1000];
  int i, j;
  double mx, mi, biw, pct;
  unsigned char *bits24;

  imT = new double[sx * sx * 3];
  memcpy(imT, data, sx * sx * 3 * sizeof(double));

  // Post processing HDR map - find reasonable cutoffs for normalization
  for (j = 0; j < 1000; j++) HDRhist[j] = 0;

  mi = 10e6;
  mx = -10e6;
  for (i = 0; i < sx * sx * 3; i++) {
    if (*(imT + i) < mi) mi = *(imT + i);
    if (*(imT + i) > mx) mx = *(imT + i);
  }

  for (i = 0; i < sx * sx * 3; i++) {
    *(imT + i) = *(imT + i) - mi;
    *(imT + i) = *(imT + i) / (mx - mi);
  }

  biw = 1.000001 / 1000.0;
  // Histogram
  for (i = 0; i < sx * sx * 3; i++) {
    for (j = 0; j < 1000; j++)
      if (*(imT + i) >= (biw * j) && *(imT + i) < (biw * (j + 1))) {
        HDRhist[j]++;
        break;
      }
  }

  pct = .005 * (sx * sx * 3);
  mx = 0;
  for (j = 5; j < 990; j++) {
    mx += HDRhist[j];
    if (HDRhist[j + 5] - HDRhist[j - 5] > pct) break;
    if (mx > pct) break;
  }
  mi = (biw * (.90 * j));

  for (j = 990; j > 5; j--) {
    if (HDRhist[j - 5] - HDRhist[j + 5] > pct) break;
  }
  mx = (biw * (j + (.25 * (999 - j))));

  for (i = 0; i < sx * sx * 3; i++) {
    *(imT + i) = *(imT + i) - mi;
    *(imT + i) = *(imT + i) / (mx - mi);
    if (*(imT + i) < 0.0) *(imT + i) = 0.0;
    if (*(imT + i) > 1.0) *(imT + i) = 1.0;
    *(imT + i) = pow(*(imT + i), .75);
  }

  bits24 = new unsigned char[sx * sx * 3];
  for (int i = 0; i < sx * sx * 3; i++)
    *(bits24 + i) = (unsigned char)(255.0 * (*(imT + i)));
  
  f = fopen(fname, "wb+");
  if (f == NULL) {
    fprintf(stderr, "Unable to open file %s for output.\n", fname);
    return;
  }
  fprintf(f, "P6\n");
  fprintf(f, "%d %d\n", sx, sx);
  fprintf(f, "255\n");
  fwrite(bits24, sx * sx * 3 * sizeof(unsigned char), 1, f);
  fclose(f);

  delete[] bits24;
  delete[] imT;
  return;
}