#include <cassert>
#include <algorithm>
#include "sigen/common/voxel.h"
namespace sigen {
Voxel::Voxel(int x, int y, int z)
    : x_(x), y_(y), z_(z), flag_(false), label_(-1) {}
void Voxel::AddConection(Voxel *p) {
  assert(std::find(adjacent_.begin(), adjacent_.end(), p) == adjacent_.end());
  adjacent_.push_back(p);
}
}
