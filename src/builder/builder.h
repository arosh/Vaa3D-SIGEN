#pragma once
#include "../common/point_link.h"
namespace sigen {
class builder {
  std::vector<std::shared_ptr<point_link *>> data_;
public:
  builder(std::vector<std::shared_ptr<point_link *>> data);
};
};
