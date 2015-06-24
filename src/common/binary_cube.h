#pragma once
#include <vector>
namespace sigen {
class binary_cube {
public:
  // bool[][][]
  std::vector<std::vector<std::vector<bool>>> data;
  int x_, y_, z_;
  binary_cube(int x, int y, int z);
  std::vector<std::vector<bool>> &operator [](int index);
};
}
