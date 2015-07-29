#include "./point.h"
#include <tuple>
namespace sigen {
point::point(int x, int y, int z) : x_(x), y_(y), z_(z), flag_(false), label_(-1) {}
void point::add_connection(point *p) {
  adjacent_.push_back(p);
}
}
