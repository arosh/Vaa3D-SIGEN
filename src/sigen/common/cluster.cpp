#include <cstdlib>
#include <vector>
#include <boost/foreach.hpp>
#include "sigen/common/cluster.h"
namespace sigen {
cluster::cluster(const std::vector<point<int> > &ps) : points_(ps) {}
bool cluster::check_neighbor(const cluster *other) {
  for (const point<int> &p : points_) {
    for (const point<int> &q : other->points_) {
      int dx = std::abs(p.x_ - q.x_);
      int dy = std::abs(p.y_ - q.y_);
      int dz = std::abs(p.z_ - q.z_);
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
