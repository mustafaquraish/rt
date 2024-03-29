#include <util/image.h>

#include <util/image/ppm.h>
#include <util/image/pfm.h>
#include <util/image/png.h>
#include <util/image/bmp.h>
#include <util/timer.h>

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

auto get_extension(std::string_view fn) {
  return fn.substr(fn.find_last_of(".") + 1);
}

/******************************************************************************/

Image::Image(int sx, int sy) : sx(sx), sy(sy) {
  m_data = new float[sx * sy * 3];
}

Image::Image(char const *fname) {
  auto extension = get_extension(fname);
  if      (extension == "ppm") { PPM::load(*this, fname); }
  else if (extension == "pfm") { PFM::load(*this, fname); }
  else if (extension == "png") { PNG::load(*this, fname); }
  else if (extension == "bmp") { BMP::load(*this, fname); }
  else { printf("[-] File '%s' unsupported\n", fname); exit(1); }

}

Vec3 Image::get(int i, int j) {
  return Vec3(
      m_data[(i + j * sx) * 3 + 0],
      m_data[(i + j * sx) * 3 + 1],
      m_data[(i + j * sx) * 3 + 2]
  );
}

Vec3 Image::get(float u, float v) {
  u = modulus(u, 1.0f);
  v = modulus(v, 1.0f);
  
  float dx = clamp(u * (sx-1), 0, sx-1) ;
  float dy = clamp(v * (sy-1), 0, sy-1) ;

  int f_x = dx, c_x = ceil(dx);
  int f_y = dy, c_y = ceil(dy);

  return bilerp(dx - f_x, dy - f_y, get(f_x, f_y), get(c_x, f_y),
                                    get(f_x, c_y), get(c_x, c_y));
}

float linearToSRGB(float L) {
  float S = L * 12.92;
  if (L > 0.0031308) {
    S = 1.055 * pow(L, 1.0 / 2.4) - 0.055;
  }
  return clamp01(S);
}

void Image::save(char const *fname, bool gammaCorrect, float exposure) {
  Timer timer("Saving '%s'", fname);
  
  float *temp  = new float[sx * sy * 3];
  std::copy(m_data, m_data + sx * sy * 3, temp);

  for (int i = 0; i < sx * sy * 3; i++) {
    if (!gammaCorrect) {
      m_data[i] = clamp01(m_data[i] * exposure);
    } else {
      m_data[i] = linearToSRGB(m_data[i] * exposure);
    }
  }
  auto extension = get_extension(fname);
  if      (extension == "ppm") { PPM::save(*this, fname); }
  else if (extension == "pfm") { PFM::save(*this, fname); }
  else if (extension == "png") { PNG::save(*this, fname); }
  else if (extension == "bmp") { BMP::save(*this, fname); }
  else { printf("[-] File '%s' unsupported\n", fname); exit(1); }

  std::copy(temp, temp + sx * sy * 3, m_data);
  delete[] temp;
}

void Image::set(int i, int j, Colour col) {
  m_data[(i + j * sx) * 3 + 0] = col.x;
  m_data[(i + j * sx) * 3 + 1] = col.y;
  m_data[(i + j * sx) * 3 + 2] = col.z;
}

void Image::splat(int i, int j, Colour col) {
  m_data[(i + j * sx) * 3 + 0] += col.x;
  m_data[(i + j * sx) * 3 + 1] += col.y;
  m_data[(i + j * sx) * 3 + 2] += col.z;
}

Image::~Image() { 
  delete[] m_data;
}