#pragma once
#include "voxel.h"
#include "point.h"
#include <vector>
#include <memory>
#include <boost/utility.hpp>
namespace sigen {
class cluster : boost::noncopyable {
public:
  double gx_, gy_, gz_;
  double radius_;
  std::vector<point<int>> points_;
  std::vector<cluster *> adjacent_;
  explicit cluster(const std::vector<point<int>> &points);
  bool check_neighbor(const cluster *p);
  void add_connection(cluster *p);
};
} // namespace sigen
