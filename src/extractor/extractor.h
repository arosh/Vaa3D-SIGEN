#include "../common/binary_cube.h"
#include "../common/point_link.h"
#include "../common/voxel.h"
#include <vector>
#include <memory>
namespace sigen {
class extractor {
  void labeling();
  std::vector<std::shared_ptr<voxel>> voxel_owner_;

public:
  binary_cube cube_;
  std::vector<std::vector<voxel *>> clusters_;
  explicit extractor(const binary_cube &cube);
  std::vector<std::shared_ptr<point_link>> extract();
};
} // namespace sigen
