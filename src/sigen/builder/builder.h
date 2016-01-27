#pragma once
#include "../common/cluster.h"
#include "../common/neuron.h"
#include <boost/utility.hpp>
#include <vector>
namespace sigen {
class Builder : boost::noncopyable {
  bool is_radius_computed_;

public:
  std::vector<boost::shared_ptr<Cluster> > data_;
  double scale_xy_, scale_z_;
  explicit Builder(const std::vector<boost::shared_ptr<Cluster> > &data,
                   const double scale_xy, const double scale_z);
  std::vector<Neuron> build();
  static std::vector<Neuron>
  convert_to_neuron(std::vector<boost::shared_ptr<Cluster> > &data,
                    const double scale_xy, const double scale_z);
  static std::vector<NeuronNodePtr>
  convert_to_neuron_node(std::vector<boost::shared_ptr<Cluster> > &data,
                         const double scale_xy, const double scale_z);
  static void compute_node_type(std::vector<Neuron> &neu);
  static void compute_id(std::vector<Neuron> &neu);
  void connect_neighbor();
  void cut_loops();
  void compute_gravity_point();
  void compute_radius();
};
} // namespace sigen
