#include <cstdlib>
#include <vector>
#include <boost/foreach.hpp>
#include "sigen/common/cluster.h"
namespace sigen {
Cluster::Cluster(const std::vector<Point<int> > &ps) : points_(ps) {}
// This functions is HOT SPOT.
// This functions is called too many.
// This is worth to tune.
bool Cluster::check_neighbor(const Cluster *other) {
  for (const Point<int> &p : points_) {
    for (const Point<int> &q : other->points_) {
      int dx = std::abs(p.x_ - q.x_);
      int dy = std::abs(p.y_ - q.y_);
      int dz = std::abs(p.z_ - q.z_);
      if (dx <= 1 && dy <= 1 && dz <= 1)
        return true;
    }
  }
  return false;
}
void Cluster::add_connection(Cluster *other) {
  adjacent_.push_back(other);
}
} // namespace sigen
