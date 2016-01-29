#pragma once
#include <vector>
namespace sigen {
// this class represents bool[][][]
class BinaryCube {
public:
  int x_, y_, z_;
  std::vector<std::vector<std::vector<bool> > > data_;
  BinaryCube(int x, int y, int z);
  // setter
  inline std::vector<std::vector<bool> > &operator[](int index) {
    return data_[index];
  }
  // getter
  inline const std::vector<std::vector<bool> > &operator[](int index) const {
    return data_[index];
  }
  void Clear();
};
} // namespace sigen
