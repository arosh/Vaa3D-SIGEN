#pragma once
#include <vector>
#include <boost/utility.hpp>
#include "sigen/common/voxel.h"
#include "sigen/common/point.h"
namespace sigen {
class Cluster : boost::noncopyable {
public:
  double gx_, gy_, gz_;
  double radius_;
  std::vector<point<int> > points_;
  std::vector<Cluster *> adjacent_;
  explicit Cluster(const std::vector<point<int> > &points);
  bool check_neighbor(const Cluster *p);
  void add_connection(Cluster *p);
};
} // namespace sigen
