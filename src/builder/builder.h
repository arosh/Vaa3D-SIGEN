#pragma once
#include "../common/point_link.h"
#include "../common/neuron.h"
namespace sigen {
class builder {
  std::vector<std::shared_ptr<point_link>> data_;
  void connect_neighbor();
  void connect_interpolate(double dt);
  neuron *find_edge();

public:
  builder(const std::vector<std::shared_ptr<point_link>> &data);
  std::pair<std::vector<std::shared_ptr<neuron>>, neuron *> build();
};
};
