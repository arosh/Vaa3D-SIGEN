#pragma once
#include "./variant.h"
#include <vector>
#include <map>
#include <memory>
namespace sigen {
enum class neuron_type {
  EDGE,
  BRANCH,
  CONNECT
};
class neuron_part {
public:
  // 1-based
  int id_;
  // gravity point
  double gx_, gy_, gz_;
  double radius_;
  neuron_type type_;
  neuron_part *parent_ = nullptr;
  std::vector<neuron_part *> childs_;
  // Variant may contain `degree`, `real_distance`, `electrical_distance`
  std::map<std::string, Variant> values_;
  void add_child(neuron_part *child);
};
typedef std::vector<std::shared_ptr<neuron_part>> neuron;
};
