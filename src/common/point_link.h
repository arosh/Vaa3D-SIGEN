#pragma once

#include "../extractor/point.h"
#include <vector>
#include <memory>

namespace sigen {
class point_link {
public:
  std::vector<point *> points_;
  std::vector<point_link *> adjacent_;
  point_link(const std::vector<point *> points);
  void add_connection(point_link *pl);
};
}
