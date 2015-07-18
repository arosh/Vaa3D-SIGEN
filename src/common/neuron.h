#pragma once
#include "./variant.h"
#include <vector>
#include <map>
namespace sigen {
enum class neuron_type {
  EDGE,
  BRANCH,
  CONNECT
};
class neuron {
public:
  // 1-based
  int id_;
  // gravity point
  double gx_, gy_, gz_;
  neuron_type type_;
  neuron* parent_ = nullptr;
  std::vector<neuron *> childs_;
  double radius_;
  // Variant may contain `degree`, `real_distance`, `electrical_distance`
  std::map<std::string, Variant> values_;
  void add_child(neuron *child);
};
};
