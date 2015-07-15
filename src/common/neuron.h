#pragma once
#include <vector>
#include <memory>
namespace sigen {
enum class neuron_node_type {
  EDGE, BRANCH, CONNECT
};
class neuron_node {
public:
  int id_;
  // gravity point
  double gx, gy, gz;
  neuron_node_type type_;
  std::vector<std::shared_ptr<neuron_node>> adjacent_;
  double real_distance_;
  double electrical_distance_;
  double radius_;
  double degree_;
};
class neuron {
};
};
