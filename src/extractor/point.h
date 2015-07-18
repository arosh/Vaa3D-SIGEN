#pragma once
#include <vector>
namespace sigen {
class point {
public:
  int x_, y_, z_;
  bool flag_; // any functions can use this variable
  int label_; // any functions can use this variable
  std::vector<point *> adjacent_;
  point(int x, int y, int z);
  void add_connection(point *p);
};
}
