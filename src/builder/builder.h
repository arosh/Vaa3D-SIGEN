#pragma once
#include "../common/cluster.h"
#include "../common/neuron.h"
#include <vector>
#include <utility>
namespace sigen {
class builder {
  void connect_neighbor();
  void connect_interpolate(double dt);
  void compute_gravity_point();
  void compute_radius();

public:
  std::vector<std::shared_ptr<cluster>> data_;
  double scale_xy_, scale_z_;
  explicit builder(const std::vector<std::shared_ptr<cluster>> &data, const double scale_xy, const double scale_z);
  std::vector<neuron> build();
  static std::vector<neuron> convert_to_neuron(std::vector<std::shared_ptr<cluster>> &data, const double scale_xy, const double scale_z);
  static void compute_node_type(std::vector<neuron> &neu);
  static void compute_id(std::vector<neuron> &neu);
};
} // namespace sigen