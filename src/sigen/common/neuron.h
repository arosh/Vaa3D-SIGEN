#pragma once
#include <string>
#include <vector>
#include <map>
#include <set>
#include <boost/utility.hpp>
#include <boost/shared_ptr.hpp>
// #include "common/variant.h"
namespace sigen {
enum neuron_type {
  EDGE,
  BRANCH,
  CONNECT
};
class NeuronNode : boost::noncopyable {
public:
  // 1-based
  int id_;
  // gravity point
  double gx_, gy_, gz_;
  double radius_;
  neuron_type type_;
  std::set<NeuronNode *> adjacent_;
  // Variant may contain `degree`, `real_distance`, `electrical_distance`
  // std::map<std::string, Variant> values_;
  void add_connection(NeuronNode *node);
  void remove_connection(NeuronNode *node);
  void remove_connection(const std::set<int> &nodes);
  inline bool has_connection(NeuronNode *node) {
    return adjacent_.count(node);
  }
  inline void coord(const double gx, const double gy, const double gz) {
    gx_ = gx;
    gy_ = gy;
    gz_ = gz;
  }
};
class Neuron /* : noncopyable */ {
public:
  std::vector<boost::shared_ptr<NeuronNode> > storage_;
  NeuronNode *root_;
  Neuron clone() const;
  inline void add_node(boost::shared_ptr<NeuronNode> node) {
    storage_.push_back(node);
  }
  inline bool empty() const {
    return storage_.empty();
  }
  inline void clear() {
    root_ = NULL;
    storage_.clear();
  }
  inline void extend(const Neuron &other) {
    this->storage_.insert(this->storage_.end(), other.storage_.begin(), other.storage_.end());
  }
};
} // namespace sigen
