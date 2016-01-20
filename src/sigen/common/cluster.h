#pragma once
#include <cassert>
#include <vector>
#include <set>
#include <boost/utility.hpp>
#include "sigen/common/voxel.h"
#include "sigen/common/point.h"
namespace sigen {
class Cluster : boost::noncopyable {
public:
  double gx_, gy_, gz_;
  double radius_;
  std::vector<IPoint> points_;
  std::set<Cluster *> adjacent_;
  explicit Cluster(const std::vector<IPoint> &points);
  bool check_neighbor(const Cluster *p);
  void add_connection(const Cluster *p);
  // http://stackoverflow.com/questions/5727264
  inline bool is_connecting_with(const Cluster *p) const {
    return adjacent_.count(const_cast<Cluster *>(p));
  }
  inline void remove_connection(const Cluster *p) {
    assert(is_connecting_with(p));
    adjacent_.erase(const_cast<Cluster *>(p));
  }
};
} // namespace sigen
