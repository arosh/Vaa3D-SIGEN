#pragma once

#include <vector>
#include <memory>

namespace sigen {
class point;
typedef std::shared_ptr<point> Point;
class point {
public:
  int x_, y_, z_;
  bool flag_; // any functions can use this variable
  int label_; // any functions can use this variable
  std::vector<std::weak_ptr<point>> adjacent_;
  point(int x, int y, int z);
  void add_connection(Point p);
};
}
