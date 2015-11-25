#pragma once
#include <vector>
namespace sigen {
// this class represents bool[][][]
class binary_cube {
public:
  int x_, y_, z_;
  std::vector<std::vector<std::vector<bool> > > data_;
  binary_cube(int x, int y, int z);
  // setter
  std::vector<std::vector<bool> > &operator[](int index);
  // getter
  const std::vector<std::vector<bool> > &operator[](int index) const;
  void clear();
};
} // namespace sigen
