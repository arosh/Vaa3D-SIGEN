#include <cassert>
#include <algorithm>
#include "common/voxel.h"
namespace sigen {
voxel::voxel(int x, int y, int z)
    : x_(x), y_(y), z_(z), flag_(false), label_(-1) {}
void voxel::add_connection(voxel *p) {
  assert(std::find(adjacent_.begin(), adjacent_.end(), p) == adjacent_.end());
  adjacent_.push_back(p);
}
}
