#pragma once
#include "variant.h"
#include <vector>
#include <memory>
#include <map>
namespace sigen {
enum class neuron_node_type {
  EDGE, BRANCH, CONNECT
};
class neuron_node {
public:
  // 1-based
  int id_; 
  // gravity point
  double gx_, gy_, gz_;
  neuron_node_type type_;
  std::shared_ptr<neuron_node> parent_;
  std::vector<std::shared_ptr<neuron_node>> adjacent_;
  double radius_;
  // values_ may contain `degree`, `real_distance`, `electrical_distance`
  std::map<std::string, Variant> values_;
};
typedef std::shared_ptr<neuron_node> neuron_node_ptr;
class neuron {
public:
  neuron_node_ptr root_;
};
};
