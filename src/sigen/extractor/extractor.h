#include "sigen/common/binary_cube.h"
#include "sigen/common/cluster.h"
#include "sigen/common/voxel.h"
#include <vector>
#include <boost/utility.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
namespace sigen {
class Extractor : boost::noncopyable {
  void labeling();

public:
  BinaryCube cube_;
  std::vector<std::vector<boost::shared_ptr<Voxel> > > components_;
  explicit Extractor(const BinaryCube &cube);
  std::vector<boost::shared_ptr<Cluster> > extract();
};
} // namespace sigen
