#include "point_link.h"
namespace sigen {
point_link::point_link(const std::vector<point *> points)
  : points_(points) {}
void point_link::add_connection(point_link *pl) {
  adjacent_.push_back(pl);
}
};
