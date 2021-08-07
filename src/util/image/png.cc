#include <png.h>
#include <util/image/png.h>

namespace PNG {

/**
 * Read the given PNG file and save it into a 2D array of bytes
 */
bool load(Image &img, const char *filename) {
  FILE *fp = fopen(filename, "rb");
  if (!fp) {
    return false;
  }
  png_structp png_ptr =
      png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png_ptr) {
    fclose(fp);
    return false;
  }
  png_infop info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr) {
    png_destroy_read_struct(&png_ptr, NULL, NULL);
    fclose(fp);
    return false;
  }
  if (setjmp(png_jmpbuf(png_ptr))) {
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
    return false;
  }
  png_init_io(png_ptr, fp);
  png_read_info(png_ptr, info_ptr);
  png_uint_32 width, height;
  int bit_depth, color_type, interlace_type;
  png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
               &interlace_type, NULL, NULL);
  if (color_type == PNG_COLOR_TYPE_PALETTE) {
    png_set_palette_to_rgb(png_ptr);
  }
  if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) {
    png_set_expand_gray_1_2_4_to_8(png_ptr);
  }
  if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
    png_set_tRNS_to_alpha(png_ptr);
  }
  if (bit_depth == 16) {
    png_set_strip_16(png_ptr);
  }
  if (bit_depth < 8) {
    png_set_packing(png_ptr);
  }
  if (color_type == PNG_COLOR_TYPE_GRAY ||
      color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
    png_set_gray_to_rgb(png_ptr);
  }
  png_read_update_info(png_ptr, info_ptr);
  int rowbytes = png_get_rowbytes(png_ptr, info_ptr);
  int channels = png_get_channels(png_ptr, info_ptr);
  int size = rowbytes * height;

  png_byte *image = new png_byte[size];
  png_bytep row_pointers[height];
  for (int i = 0; i < height; i++) {
    row_pointers[i] = &image[i * rowbytes];
  }
  png_read_image(png_ptr, row_pointers);
  fclose(fp);
  png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

  img.sx = width;
  img.sy = height;
  img.m_data = new float[size];
  for (int i = 0; i < size; i++) {
    img.m_data[i] = (float)image[i] / 255.0;
  }
  delete[] image;
  return true;
}

/**
 * Save the image stored in `img` into the given PNG file
 */
bool save(Image &img, const char *filename) {
  FILE *fp = fopen(filename, "wb");
  if (!fp) {
    return false;
  }
  png_structp png_ptr =
      png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png_ptr) {
    fclose(fp);
    return false;
  }
  png_infop info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr) {
    png_destroy_write_struct(&png_ptr, NULL);
    fclose(fp);
    return false;
  }
  if (setjmp(png_jmpbuf(png_ptr))) {
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);
    return false;
  }
  png_init_io(png_ptr, fp);
  png_set_IHDR(png_ptr, info_ptr, img.sx, img.sy, 8, PNG_COLOR_TYPE_RGB,
               PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE,
               PNG_FILTER_TYPE_BASE);
  png_write_info(png_ptr, info_ptr);
  png_byte *image = new png_byte[img.sx * img.sy * 3];
  for (int i = 0; i < img.sx * img.sy * 3; i++) {
    image[i] = (png_byte)(img.m_data[i] * 255.0);
  }
  png_bytep row_pointers[img.sy];
  for (int i = 0; i < img.sy; i++) {
    row_pointers[i] = &image[i * img.sx * 3];
  }
  png_write_image(png_ptr, row_pointers);
  png_write_end(png_ptr, info_ptr);
  png_destroy_write_struct(&png_ptr, &info_ptr);
  fclose(fp);

  delete[] image;
  return true;
}

}  // namespace PNG