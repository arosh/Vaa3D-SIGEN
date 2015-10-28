#pragma once
#include <vector>
#include <stdint.h>

class Image3D {
public:
  int x_, y_, z_;
  // uint8_t[][][]
  std::vector<std::vector<std::vector<uint8_t> > > data_;
  Image3D(int x, int y, int z);
  std::vector<std::vector<uint8_t> > &operator[](int index);
  void clear();
};
