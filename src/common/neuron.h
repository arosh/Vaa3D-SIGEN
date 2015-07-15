#pragma once
#include <vector>
#include <memory>
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
  double real_distance_;
  double electrical_distance_;
  double radius_;
  double degree_;
};
typedef std::shared_ptr<neuron_node> neuron_node_ptr;
class neuron {
public:
  neuron_node_ptr root_;
};
};