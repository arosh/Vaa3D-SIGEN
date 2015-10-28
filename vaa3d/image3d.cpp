#include "image3d.h"

Image3D::Image3D(int x, int y, int z)
    : x_(x), y_(y), z_(z),
      data_(x, std::vector<std::vector<uint8_t> >(y, std::vector<uint8_t>(z))) {}

std::vector<std::vector<uint8_t> > &Image3D::operator[](int index) {
  return data_[index];
}

void Image3D::clear() {
  data_.clear();
}
