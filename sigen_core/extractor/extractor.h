#include "../common/binary_cube.h"
#include "../common/cluster.h"
#include "../common/voxel.h"
#include <vector>
#include <boost/utility.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
namespace sigen {
class extractor : boost::noncopyable {
  void labeling();

public:
  binary_cube cube_;
  std::vector<std::vector<boost::shared_ptr<voxel> > > components_;
  explicit extractor(const binary_cube &cube);
  std::vector<boost::shared_ptr<cluster> > extract();
};
} // namespace sigen
