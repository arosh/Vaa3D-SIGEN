#include "./binary_cube.h"
#include <vector>
namespace sigen {
binary_cube::binary_cube(int x, int y, int z)
    : x_(x), y_(y), z_(z),
      data_(x, std::vector<std::vector<bool>>(y, std::vector<bool>(z))) {}
std::vector<std::vector<bool>> &binary_cube::operator[](int index) {
  return data_[index];
}
void binary_cube::clear() {
  data_.clear();
}
} // namespace sigen
