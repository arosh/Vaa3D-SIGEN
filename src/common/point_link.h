#pragma once
#include "./point.h"
#include <vector>
#include <memory>
namespace sigen {
class point_link {
public:
  std::vector<point *> points_;
  std::vector<point_link *> adjacent_;
  explicit point_link(const std::vector<point *> &points);
  bool check_neighbor(const point_link *pl);
  void add_connection(point_link *pl);
};
}
