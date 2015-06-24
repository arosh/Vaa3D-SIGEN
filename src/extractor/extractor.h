#include "../common/binary_cube.h"
#include "point.h"
#include <vector>
#include <list>

namespace sigen {
class extractor {
public:
  binary_cube cube_;
  // std::vector<point>の容量が大きそうなのでstd::listで管理しておく
  std::list<std::vector<point>> clusters_;
  extractor(const binary_cube &cube);
  void labeling();
};
}
