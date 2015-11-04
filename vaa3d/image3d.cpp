#include "image3d.h"

Image3d::Image3d(int x, int y, int z)
    : x_(x), y_(y), z_(z),
      data_(x, std::vector<std::vector<bool> >(y, std::vector<bool>(z))) {}

std::vector<std::vector<bool> > &Image3d::operator[](int index) {
  return data_[index];
}

const std::vector<std::vector<bool> > &Image3d::operator[](int index) const {
  return data_[index];
}

void Image3d::clear() {
  data_.clear();
}
