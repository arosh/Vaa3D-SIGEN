#include "point.h"
#include <tuple>
namespace sigen {
point::point(int x, int y, int z) : x_(x), y_(y), z_(z), flag_(false) {}
void point::add_connection(std::shared_ptr<point> p) {
  adjacent.push_back(p);
}
bool point::operator<(const point &rhs) const {
  return std::tie(x_, y_, z_) < std::tie(rhs.x_, rhs.y_, rhs.z_);
}
}
