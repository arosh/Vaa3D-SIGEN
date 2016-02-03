#pragma once
#include <string>
#include <vector>
#include <map>
#include <set>
#include <boost/utility.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/foreach.hpp>
// #include "common/variant.h"
namespace sigen {
namespace NeuronType {
enum enum_t { EDGE,
              BRANCH,
              CONNECT };
}
class NeuronNode;
typedef boost::shared_ptr<NeuronNode> NeuronNodePtr;
class NeuronNode : boost::noncopyable {
public:
  // 1-based
  int id_;
  // gravity point
  double gx_, gy_, gz_;
  double radius_;
  NeuronType::enum_t type_;
  std::set<NeuronNode *> adjacent_;
  // Variant may contain `degree`, `real_distance`, `electrical_distance`
  // std::map<std::string, Variant> values_;

  void AddConnection(NeuronNode *node) {
    assert(!adjacent_.count(node));
    adjacent_.insert(node);
  }
  void AddConnection(NeuronNodePtr node) {
    this->AddConnection(node.get());
  }
  void RemoveConnection(NeuronNode *node) {
    assert(adjacent_.count(node));
    adjacent_.erase(node);
  }

  void RemoveConnection(const std::set<int> &nodes);

  bool HasConnection(NeuronNode *node) const {
    return adjacent_.count(node) > 0;
  }
  bool HasConnection(NeuronNodePtr node) const {
    return this->HasConnection(node.get());
  }
  void setCoord(const double gx, const double gy, const double gz) {
    gx_ = gx;
    gy_ = gy;
    gz_ = gz;
  }
  NeuronNodePtr Clone() const {
    NeuronNodePtr r = boost::make_shared<NeuronNode>();
    r->id_ = id_;
    r->setCoord(gx_, gy_, gz_);
    r->radius_ = radius_;
    r->type_ = type_;
    return r;
  }
  int CountNumChild(const NeuronNode *parent) const {
    int count = 0;
    BOOST_FOREACH (const NeuronNode *next, this->adjacent_) {
      if (next != parent) {
        count++;
      }
    }
    return count;
  }
};
class Neuron /* : noncopyable */ {
  NeuronNode *root_;
public:
  std::vector<NeuronNodePtr> storage_;

  Neuron Clone() const;

  NeuronNode *get_root() const { return root_; }
  void set_root(NeuronNode *value) { root_ = value; }
  void AddNode(NeuronNodePtr node) {
    storage_.push_back(node);
  }
  bool IsEmpty() const {
    return storage_.empty();
  }
  void Clear() {
    root_ = NULL;
    storage_.clear();
  }
  void Extend(const Neuron &other) {
    this->storage_.insert(this->storage_.end(), other.storage_.begin(), other.storage_.end());
  }
  void UpdateRoot(const int nth) {
    assert(0 <= nth && nth < (int)storage_.size());
    root_ = storage_[nth].get();
  }
  void RemoveConnection(const std::set<int> &nodes) {
    BOOST_FOREACH (NeuronNodePtr p, storage_) {
      p->RemoveConnection(nodes);
    }
  }
};
} // namespace sigen
