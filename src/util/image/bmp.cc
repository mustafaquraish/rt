#include <assert.h>
#include <util/image.h>
#include <util/image/bmp.h>

namespace BMP {

/**
 * Based on documentation at:
 *  https://docs.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapfileheader
 */
typedef struct bmpfileheader {
  uint16_t bf_type = 0x4d42;
  uint32_t bf_size = 0;
  uint16_t bf_reserved_1 = 0;
  uint16_t bf_reserved_2 = 0;
  uint32_t bf_offset_bits = 54;
} __attribute__((__packed__)) BMPFileHeader;

/**
 * Based on documentation at:
 * https://docs.microsoft.com/en-us/previous-versions//dd183376(v=vs.85)
 */
typedef struct bmpinfoheader {
  uint32_t bi_size = 40;
  int32_t bi_width = 0;
  int32_t bi_height = 0;
  uint16_t bi_planes = 1;
  uint16_t bi_bit_count = 24;
  uint32_t bi_compression = 0;
  uint32_t bi_size_image = 0;
  int32_t bi_x_px_meter = 0;
  int32_t bi_y_px_meter = 0;
  uint32_t bi_clr_used = 0;
  uint32_t bi_clr_important = 0;
} __attribute__((__packed__)) BMPInfoHeader;

bool load(Image &img, const char *filename) {
  // Open input file
  FILE *f = fopen(filename, "rb");
  if (f == NULL) {
    std::cerr << "Error: Unable to open file " << filename << " for reading\n";
    return false;
  }

  std::cout << "Loading " << filename << "..." << std::endl;

  // Read the file and info headers from the file.
  BMPFileHeader bf;
  BMPInfoHeader bi;

  assert(fread(&bf, sizeof(BMPFileHeader), 1, f));
  assert(fread(&bi, sizeof(BMPInfoHeader), 1, f));

  // Ensure file is (likely) a 24-bit uncompressed BMP 4.0
  if (bf.bf_type != 0x4d42 || bf.bf_offset_bits != 54 || bi.bi_size != 40 ||
      bi.bi_bit_count != 24 || bi.bi_compression != 0) {
    fclose(f);
    std::cerr << "Incorrect file format, use BMP 4.0 24-bit.\n";
    return false;
  }

  int sy = abs(bi.bi_height);
  int sx = abs(bi.bi_width);
  uint8_t *bits24 = new uint8_t[sx * sy * 3];

  // Determine padding for scanlines
  int padding = (4 - (sx * 3) % 4) % 4;

  // Iterate over file's scanlines
  for (int i = 0; i < sy; i++) {
    // Read row into pixel array
    assert(fread(&bits24[i * sx * 3], 3, sx, f));
    // Skip over padding
    fseek(f, padding, SEEK_CUR);
  }

  img.sx = sx;
  img.sy = sy;
  img.m_data = new float[img.sx * img.sy * 3];
  for (int i = 0; i < sx; i++) {
    for (int j = 0; j < sy; j++) {
      // Rows are top-to-bottom
      int data_index = i + j * sx;
      int img_index = i + (img.sy - j - 1) * img.sx;
      // Pixels stored in BGR format
      img.m_data[img_index * 3 + 0] = bits24[data_index * 3 + 2] / 255.0;
      img.m_data[img_index * 3 + 1] = bits24[data_index * 3 + 1] / 255.0;
      img.m_data[img_index * 3 + 2] = bits24[data_index * 3 + 0] / 255.0;
    }
  }
  fclose(f);
  delete[] bits24;
  return true;
}

bool save(Image &img, const char *filename) {
  FILE *f;
  unsigned char *bits24 = new unsigned char[img.sx * img.sy * 3];

  for (int i = 0; i < img.sx; i++) {
    for (int j = 0; j < img.sy; j++) {
      // Rows are top-to-bottom
      int data_index = i + j * img.sx;
      int img_index = i + (img.sy - j - 1) * img.sx;
      // Pixels stored in BGR format
      bits24[data_index * 3 + 2] = img.m_data[img_index * 3 + 0] * 255.0;
      bits24[data_index * 3 + 1] = img.m_data[img_index * 3 + 1] * 255.0;
      bits24[data_index * 3 + 0] = img.m_data[img_index * 3 + 2] * 255.0;
    }
  }

  BMPFileHeader bf;
  BMPInfoHeader bi;
  bf.bf_size = img.sx * img.sy * 3 + 54;
  bi.bi_size_image = img.sx * img.sy * 3;
  bi.bi_width = img.sx;
  bi.bi_height = img.sy;

  f = fopen(filename, "wb+");
  if (f == NULL) {
    std::cerr << "Unable to open file " << filename << "\n";
    return false;
  }
  // Write file header
  assert(fwrite(&bf, sizeof(BMPFileHeader), 1, f));
  assert(fwrite(&bi, sizeof(BMPInfoHeader), 1, f));
  // Write image data
  assert(fwrite(bits24, img.sx * img.sy * 3, 1, f));
  fclose(f);
  delete[] bits24;
  return true;
}

}  // namespace BMP