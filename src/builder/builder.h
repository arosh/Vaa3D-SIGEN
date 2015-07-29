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

public:
  explicit builder(const std::vector<std::shared_ptr<cluster>> &data);
  neuron build();
};
} // namespace sigen
