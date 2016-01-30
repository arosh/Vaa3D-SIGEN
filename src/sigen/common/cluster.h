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
  explicit Cluster(const std::vector<IPoint> &points) : gx_(0.0), gy_(0.0), gz_(0.0), radius_(0.0), points_(points) {}

  // http://stackoverflow.com/questions/5727264
  bool HasConnection(const Cluster *p) const {
    return adjacent_.count(const_cast<Cluster *>(p)) > 0; // FIXME please remove const_cast
  }
  bool HasConnection(ClusterPtr p) const {
    return this->HasConnection(p.get());
  }
  void RemoveConnection(const Cluster *p) {
    assert(this->HasConnection(p));
    adjacent_.erase(const_cast<Cluster *>(p)); // FIXME please remove const_cast
  }
  void AddConnection(const Cluster *p) {
    assert(!this->HasConnection(p));
    adjacent_.insert(const_cast<Cluster *>(p)); // FIXME please remove const_cast
  }
  void AddConnection(ClusterPtr p) {
    this->AddConnection(p.get());
  }
};
} // namespace sigen
