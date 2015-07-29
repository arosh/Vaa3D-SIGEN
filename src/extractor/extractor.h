#include "../common/binary_cube.h"
#include "../common/cluster.h"
#include "../common/voxel.h"
#include <vector>
#include <memory>
namespace sigen {
class extractor {
  void labeling();
  std::vector<std::shared_ptr<voxel>> voxel_owner_;

public:
  binary_cube cube_;
  std::vector<std::vector<voxel *>> groups_;
  explicit extractor(const binary_cube &cube);
  std::vector<std::shared_ptr<cluster>> extract();
};
} // namespace sigen
