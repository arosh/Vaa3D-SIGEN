#pragma once

#include <vector>

namespace sigen {
class binary_cube {
public:
  int x_, y_, z_;
  // bool[][][]
  std::vector<std::vector<std::vector<bool>>> data_;
  binary_cube(int x, int y, int z);
  std::vector<std::vector<bool>> &operator[](int index);
};
}
