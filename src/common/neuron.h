#pragma once
#include "./variant.h"
#include <string>
#include <vector>
#include <map>
#include <memory>
namespace sigen {
enum class neuron_type {
  EDGE,
  BRANCH,
  CONNECT
};
class neuron_node {
public:
  // 1-based
  int id_;
  // gravity point
  double gx_, gy_, gz_;
  double radius_;
  neuron_type type_;
  std::vector<neuron_node *> adjacent_;
  // Variant may contain `degree`, `real_distance`, `electrical_distance`
  std::map<std::string, Variant> values_;
  void add_connection(neuron_node *node);
};
class neuron {
public:
  bool is_finalized = false;
  neuron_node *root_;
  std::vector<std::shared_ptr<neuron_node>> data_; // <- storage??
};
} // namespace sigen
