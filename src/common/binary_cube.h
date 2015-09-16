#pragma once
#include <vector>
namespace sigen {
// this class presents bool[][][]
class binary_cube {
public:
  int x_, y_, z_;
  // bool[][][]
  std::vector<std::vector<std::vector<bool>>> data_;
  binary_cube(int x, int y, int z);
  // is const accessor necessary???
  std::vector<std::vector<bool>> &operator[](int index);
  void clear();
};
} // namespace sigen
