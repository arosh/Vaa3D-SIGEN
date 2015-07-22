#pragma once

#include "../extractor/point.h"
#include <vector>
#include <memory>

namespace sigen {
class connection {
public:
  std::vector<std::vector<point *>> nodelist;
};
}
