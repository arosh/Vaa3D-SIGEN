#pragma once
#include <vector>
#include <stdint.h>

class Image3d {
public:
  int x_, y_, z_;
  // uint8_t[][][]
  std::vector<std::vector<std::vector<bool> > > data_;
  Image3d(int x, int y, int z);
  std::vector<std::vector<bool> > &operator[](int index);
  const std::vector<std::vector<bool> > &operator[](int index) const;
  void clear();
};
