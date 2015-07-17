#pragma once
#include "./variant.h"
#include <vector>
#include <memory>
#include <map>
namespace sigen {
enum class neuron_type {
  EDGE,
  BRANCH,
  CONNECT
};
class neuron;
typedef std::shared_ptr<neuron> Neuron;
class neuron {
  Neuron me_;
public:
  // 1-based
  int id_;
  // gravity point
  double gx_, gy_, gz_;
  neuron_type type_;
  std::weak_ptr<neuron> parent_;
  std::vector<std::weak_ptr<neuron>> childs_;
  double radius_;
  // Variant may contain `degree`, `real_distance`, `electrical_distance`
  std::map<std::string, Variant> values_;
  static void connect(Neuron parent, Neuron child);
};
};
