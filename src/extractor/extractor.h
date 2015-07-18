#include "../common/binary_cube.h"
#include "./point.h"
#include <vector>
#include <memory>

namespace sigen {
class extractor {
  void labeling();
  std::vector<std::shared_ptr<point>> point_owner_;
public:
  binary_cube cube_;
  std::vector<std::vector<point *>> clusters_;
  extractor(const binary_cube &cube);
  void extract();
};
}
