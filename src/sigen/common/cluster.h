#pragma once
#include <cassert>
#include <vector>
#include <set>
#include <boost/utility.hpp>
#include <boost/shared_ptr.hpp>
#include "sigen/common/voxel.h"
#include "sigen/common/point.h"
namespace sigen {
class Cluster;
typedef boost::shared_ptr<Cluster> ClusterPtr;
class Cluster : boost::noncopyable {
public:
  double gx_, gy_, gz_;
  double radius_;
  std::vector<IPoint> points_;
  std::set<Cluster *> adjacent_;
  inline explicit Cluster(const std::vector<IPoint> &points) : points_(points) {}
  bool check_neighbor(const Cluster *p);
  // http://stackoverflow.com/questions/5727264
  inline bool is_connecting_with(const Cluster *p) const {
    return adjacent_.count(const_cast<Cluster *>(p)); // FIXME please remove const_cast
  }
  inline bool is_connecting_with(ClusterPtr p) const {
    return is_connecting_with(p.get());
  }
  inline void remove_connection(const Cluster *p) {
    assert(is_connecting_with(p));
    adjacent_.erase(const_cast<Cluster *>(p)); // FIXME please remove const_cast
  }
  inline void add_connection(const Cluster *p) {
    adjacent_.insert(const_cast<Cluster *>(p)); // FIXME please remove const_cast
  }
};
} // namespace sigen
