#include "./cluster.h"
#include <cstdlib>
#include <vector>
namespace sigen {
cluster::cluster(const std::vector<voxel *> &voxels)
    : voxels_(voxels) {}
bool cluster::check_neighbor(const cluster *other) {
  for (const voxel *p : voxels_) {
    for (const voxel *q : other->voxels_) {
      int dx = std::abs(p->x_ - q->x_);
      int dy = std::abs(p->y_ - q->y_);
      int dz = std::abs(p->z_ - q->z_);
      if (dx <= 1 && dy <= 1 && dz <= 1)
        return true;
    }
  }
  return false;
}
void cluster::add_connection(cluster *other) {
  adjacent_.push_back(other);
}
} // namespace sigen
