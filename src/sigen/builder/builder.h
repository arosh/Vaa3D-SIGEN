#pragma once
#include "../common/cluster.h"
#include "../common/neuron.h"
#include <boost/utility.hpp>
#include <vector>
namespace sigen {
class builder : boost::noncopyable {
  bool is_radius_computed_;

public:
  std::vector<boost::shared_ptr<cluster> > data_;
  double scale_xy_, scale_z_;
  explicit builder(const std::vector<boost::shared_ptr<cluster> > &data,
                   const double scale_xy, const double scale_z);
  std::vector<neuron> build();
  static std::vector<neuron>
  convert_to_neuron(std::vector<boost::shared_ptr<cluster> > &data,
                    const double scale_xy, const double scale_z);
  static std::vector<boost::shared_ptr<neuron_node> >
  convert_to_neuron_node(std::vector<boost::shared_ptr<cluster> > &data,
                         const double scale_xy, const double scale_z);
  static void compute_node_type(std::vector<neuron> &neu);
  static void compute_id(std::vector<neuron> &neu);
  void connect_neighbor();
  void cut_loops();
  void connect_interpolate(double dt);
  void compute_gravity_point();
  void compute_radius();
};
} // namespace sigen
