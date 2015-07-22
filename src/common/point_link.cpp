#include "./point_link.h"
#include <cstdlib>
namespace sigen {
point_link::point_link(const std::vector<point *> points)
    : points_(points) {}
bool point_link::check_neighbor(const point_link *pl) {
  for (const point *p : points_) {
    for (const point *q : pl->points_) {
      int dx = std::abs(p->x_ - q->x_);
      int dy = std::abs(p->y_ - q->y_);
      int dz = std::abs(p->z_ - q->z_);
      if (dx <= 1 && dy <= 1 && dz <= 1) return true;
    }
  }
  return false;
}
void point_link::add_connection(point_link *pl) {
  adjacent_.push_back(pl);
}
};
