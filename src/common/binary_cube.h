#pragma once
#include <vector>
namespace sigen {
class binary_cube {
public:
  std::vector<std::vector<std::vector<bool>>> data;
  int x_, y_, z_;
  binary_cube(int x, int y, int z) :
    x_(x), y_(y), z_(z),
    data(x, std::vector<std::vector<bool>>(y, std::vector<bool>(z))) { }
  std::vector<std::vector<bool>> &operator [](int index);
};
}
