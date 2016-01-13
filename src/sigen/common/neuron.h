#pragma once
#include <string>
#include <vector>
#include <map>
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
  std::vector<NeuronNode *> adjacent_;
  // Variant may contain `degree`, `real_distance`, `electrical_distance`
  // std::map<std::string, Variant> values_;
  void add_connection(NeuronNode *node);
  void remove_connection(NeuronNode *node);
};
class Neuron /* : noncopyable */ {
public:
  NeuronNode *root_;
  std::vector<boost::shared_ptr<NeuronNode> > storage_;
  Neuron clone() const;
};
} // namespace sigen
