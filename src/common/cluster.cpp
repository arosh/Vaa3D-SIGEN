#include "./cluster.h"
#include <cstdlib>
#include <vector>
namespace sigen {
cluster::cluster(const std::vector<voxel *> &voxels)
    : voxels_(voxels) {}
bool cluster::check_neighbor(const cluster *cs) {
  for (const voxel *p : voxels_) {
    for (const voxel *q : cs->voxels_) {
      int dx = std::abs(p->x_ - q->x_);
      int dy = std::abs(p->y_ - q->y_);
      int dz = std::abs(p->z_ - q->z_);
      if (dx <= 1 && dy <= 1 && dz <= 1)
        return true;
    }
  }
  return false;
}
void cluster::add_connection(cluster *pl) {
  adjacent_.push_back(pl);
}
} // namespace sigen
