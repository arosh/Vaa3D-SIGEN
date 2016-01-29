#include <vector>
#include "sigen/common/binary_cube.h"
namespace sigen {
BinaryCube::BinaryCube(int x, int y, int z)
    : x_(x), y_(y), z_(z),
      data_(x, std::vector<std::vector<bool> >(y, std::vector<bool>(z))) {}

void BinaryCube::Clear() {
  data_.clear();
}
} // namespace sigen
