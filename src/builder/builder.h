#pragma once
#include "../common/cluster.h"
#include "../common/neuron.h"
#include <vector>
#include <utility>
namespace sigen {
class builder {
  std::vector<std::shared_ptr<cluster>> data_;
  void connect_neighbor();
  void connect_interpolate(double dt);
  neuron_node *find_edge();

public:
  explicit builder(const std::vector<std::shared_ptr<cluster>> &data);
  std::pair<std::vector<std::shared_ptr<neuron_node>>, neuron_node *> build();
};
} // namespace sigen
