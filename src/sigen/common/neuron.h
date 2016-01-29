#pragma once
#include <string>
#include <vector>
#include <map>
#include <set>
#include <boost/utility.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
// #include "common/variant.h"
namespace sigen {
enum neuron_type {
  EDGE,
  BRANCH,
  CONNECT
};
class NeuronNode;
typedef boost::shared_ptr<NeuronNode> NeuronNodePtr;
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
  void AddConnection(NeuronNode *node);
  void RemoveConnection(NeuronNode *node);
  void RemoveConnection(const std::set<int> &nodes);
  inline bool HasConnection(NeuronNode *node) const {
    return adjacent_.count(node) > 0;
  }
  inline bool HasConnection(NeuronNodePtr node) const {
    return this->HasConnection(node.get());
  }
  inline void setCoord(const double gx, const double gy, const double gz) {
    gx_ = gx;
    gy_ = gy;
    gz_ = gz;
  }
  inline NeuronNodePtr Clone() const {
    NeuronNodePtr r = boost::make_shared<NeuronNode>();
    r->id_ = id_;
    r->setCoord(gx_, gy_, gz_);
    r->radius_ = radius_;
    r->type_ = type_;
    return r;
  }
  inline int CountNumChild(const NeuronNode *parent) const {
    int count = 0;
    for (const NeuronNode *next : this->adjacent_) {
      if (next != parent) {
        count++;
      }
    }
    return count;
  }
};
class Neuron /* : noncopyable */ {
public:
  std::vector<NeuronNodePtr> storage_;
  NeuronNode *root_;
  Neuron Clone() const;
  inline void AddNode(NeuronNodePtr node) {
    storage_.push_back(node);
  }
  inline bool IsEmpty() const {
    return storage_.empty();
  }
  inline void Clear() {
    root_ = NULL;
    storage_.clear();
  }
  inline void Extend(const Neuron &other) {
    this->storage_.insert(this->storage_.end(), other.storage_.begin(), other.storage_.end());
  }
  inline void UpdateRoot(const int nth) {
    assert(0 <= nth && nth < (int)storage_.size());
    root_ = storage_[nth].get();
  }
};
} // namespace sigen
