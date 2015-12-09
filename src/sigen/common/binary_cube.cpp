#include <vector>
#include "sigen/common/binary_cube.h"
namespace sigen {
binary_cube::binary_cube(int x, int y, int z)
    : x_(x), y_(y), z_(z),
      data_(x, std::vector<std::vector<bool> >(y, std::vector<bool>(z))) {}

void binary_cube::clear() {
  data_.clear();
}
} // namespace sigen
