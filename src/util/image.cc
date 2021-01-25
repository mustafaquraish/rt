#include "util/image.h"

Image::Image(int sx, int sy): sx(sx), sy(sy) {
  data = (double *)calloc(sx * sy * 3, sizeof(double));
  wght = (double *)calloc(sx * sy, sizeof(double));
  for (int i = 0; i < sx * sy; i++) 
    wght[i] = 1.0;
}

// The following functions for loading / saving PPM images are taken
// from an image processing library created by Francisco J Estrada.
// His academic page can be found at: http://www.cs.utoronto.ca/~strider/
// Modified to work with this Ray-Tracer

Image::Image(char const *fname) {
  FILE *f;
  char line[1024];

  char *tmp;  // | These are to keep the compiler happy. Want to ignore
  int un;     // | Value of fgets() and fread()

  f = fopen(fname, "rb+");
  if (f == NULL) {
    std::cerr << "Error: Unable to open file " << fname
              << " for reading, please check name and path\n";
    exit(1);
  }

  tmp = fgets(line, 1000, f);
  if (strcmp(line, "P6\n") != 0) {
    std::cerr << "Error: Wrong file format, not a .pgm file or header "
              << "end-of-line characters missing\n";
    fclose(f);
    exit(1);
  }

  // Skip over comments
  tmp = fgets(line, 511, f);
  while (line[0] == '#') tmp = fgets(line, 511, f);
  sscanf(&line[0], "%d %d\n", &sx, &sy);  // Read file size
  tmp = fgets(&line[0], 9, f);            // Read the remaining header line

  unsigned char *raw =
      (unsigned char *)calloc(sx * sy * 3, sizeof(unsigned char));
  // Read the data
  un = fread(raw, sx * sy * 3, sizeof(unsigned char), f);

  data = (double *)calloc(sx * sy * 3, sizeof(double));
  for (int i = 0; i < sx * sy * 3; i++) data[i] = double(raw[i]) / 255.0;

  wght = (double *)calloc(sx * sy, sizeof(double));
  for (int i = 0; i < sx * sy; i++) wght[i] = 1.0;

  un = un || tmp;  // Use both temp variables so compiler doesn't shout
  free(raw);
  fclose(f);
}

void Image::save(char const *fname) {
  FILE *f;
  unsigned char *bits24 =
      (unsigned char *)calloc(sx * sy * 3, sizeof(unsigned char));

  for (int i = 0; i < sx * sy * 3; i++) *(bits24 + i) = int8_t(255.0 * data[i]);
  f = fopen(fname, "wb+");
  if (f == NULL) {
    std::cerr << "Unable to open file " << fname << ".\n";
    return;
  }

  fprintf(f, "P6\n");
  fprintf(f, "# Output from Raytracer++.c\n");
  fprintf(f, "%d %d\n", sx, sy);
  fprintf(f, "255\n");
  fwrite(bits24, sx * sy * 3 * sizeof(unsigned char), 1, f);
  free(bits24);
  fclose(f);
  return;
}

void Image::hdrsave(char const *fname) {
  FILE *f;
  double *imT;
  double HDRhist[1000];
  int i, j;
  double mx, mi, biw, pct;
  unsigned char *bits24;

  imT = (double *)calloc(sx * sx * 3, sizeof(double));
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
  fprintf(stderr, "Image stats: Minimum=%f, maximum=%f\n", mi, mx);
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

  fprintf(stderr,
          "Limit values chosen at min=%f, max=%f... normalizing image\n", mi,
          mx);

  for (i = 0; i < sx * sx * 3; i++) {
    *(imT + i) = *(imT + i) - mi;
    *(imT + i) = *(imT + i) / (mx - mi);
    if (*(imT + i) < 0.0) *(imT + i) = 0.0;
    if (*(imT + i) > 1.0) *(imT + i) = 1.0;
    *(imT + i) = pow(*(imT + i), .75);
  }

  bits24 = (unsigned char *)calloc(sx * sx * 3, sizeof(unsigned char));
  for (int i = 0; i < sx * sx * 3; i++)
    *(bits24 + i) = (unsigned char)(255.0 * (*(imT + i)));
  f = fopen(fname, "wb+");
  if (f == NULL) {
    fprintf(stderr, "Unable to open file %s for output! No image written\n",
            fname);
    return;
  }
  fprintf(f, "P6\n");
  fprintf(f, "# Output from RayTracer++\n");
  fprintf(f, "%d %d\n", sx, sx);
  fprintf(f, "255\n");
  fwrite(bits24, sx * sx * 3 * sizeof(unsigned char), 1, f);
  fclose(f);
  return;

  free(bits24);
  free(imT);
}

void Image::set(int i, int j, const Vec &col) {
  data[(i + j * sx) * 3 + 0] = col.x > 1 ? 1 : col.x;
  data[(i + j * sx) * 3 + 1] = col.y > 1 ? 1 : col.y;
  data[(i + j * sx) * 3 + 2] = col.z > 1 ? 1 : col.z;
}

void Image::hdrset(int i, int j, const Vec &col) {
  Vec ncol = clamp01(col);
  double wt = wght[i + j * sx];
  data[(i + j * sx) * 3 + 0] += ncol.x * pow(2, -log(wt));
  data[(i + j * sx) * 3 + 1] += ncol.y * pow(2, -log(wt));
  data[(i + j * sx) * 3 + 2] += ncol.z * pow(2, -log(wt));
  wght[i + j * sx] += ncol.x + ncol.y + ncol.z;
}

Image::~Image() {
  free(data);
  free(wght);
}