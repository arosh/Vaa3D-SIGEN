#include "image3d.h"

image3d::image3d(int x, int y, int z)
    : x_(x), y_(y), z_(z), data_(new bool[x*y*z])
       {}

bool &image3d::operator()(int i, int j, int k) {
  int sx = 1;
  int sy = x_;
  int sz = x_ * y_;
  return data_[sx * i + sy * j + sz * k];
}

const bool &image3d::operator()(int i, int j, int k) const {
  int sx = 1;
  int sy = x_;
  int sz = x_ * y_;
  return data_[sx * i + sy * j + sz * k];
}

image3d::~image3d() {
  delete[] data_;
}
