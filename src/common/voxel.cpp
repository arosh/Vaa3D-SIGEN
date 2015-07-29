#include "./voxel.h"
#include <tuple>
namespace sigen {
voxel::voxel(int x, int y, int z) : x_(x), y_(y), z_(z), flag_(false), label_(-1) {}
void voxel::add_connection(voxel *p) {
  adjacent_.push_back(p);
}
}
