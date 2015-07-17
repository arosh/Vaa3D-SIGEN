#include "point.h"
#include <tuple>
namespace sigen {
point::point(int x, int y, int z) : x_(x), y_(y), z_(z), flag_(false) {}
void point::add_connection(Point p) {
  adjacent_.push_back(p);
}
}
