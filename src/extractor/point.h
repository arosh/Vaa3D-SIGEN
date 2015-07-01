#pragma once

#include <vector>
#include <memory>

namespace sigen {
class point {
public:
  int x_, y_, z_;
  bool flag_;
  int label_;
  std::vector<std::shared_ptr<point>> adjacent_;
  point(int x, int y, int z);
  void add_connection(std::shared_ptr<point> p);
};
}
