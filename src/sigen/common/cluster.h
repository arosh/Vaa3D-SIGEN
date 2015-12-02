#pragma once
#include <vector>
#include <boost/utility.hpp>
#include "sigen/common/voxel.h"
#include "sigen/common/point.h"
namespace sigen {
class cluster : boost::noncopyable {
public:
  double gx_, gy_, gz_;
  double radius_;
  std::vector<point<int> > points_;
  std::vector<cluster *> adjacent_;
  explicit cluster(const std::vector<point<int> > &points);
  bool check_neighbor(const cluster *p);
  void add_connection(cluster *p);
};
} // namespace sigen
