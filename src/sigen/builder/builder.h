#pragma once
#include "sigen/common/cluster.h"
#include "sigen/common/neuron.h"
#include <boost/utility.hpp>
#include <vector>
namespace sigen {
class Builder : boost::noncopyable {
  bool is_radius_computed_;

public:
  std::vector<ClusterPtr> data_;
  double scale_xy_, scale_z_;
  explicit Builder(const std::vector<ClusterPtr> &data,
                   const double scale_xy, const double scale_z);
  std::vector<Neuron> Build();
  static std::vector<Neuron>
  ConvertToNeuron(std::vector<ClusterPtr> &data,
                  const double scale_xy, const double scale_z);
  static std::vector<NeuronNodePtr>
  ConvertToNeuronNodes(std::vector<ClusterPtr> &data,
                       const double scale_xy, const double scale_z);
  static void ComputeNodeTypes(std::vector<Neuron> &neu);
  static void ComputeIds(std::vector<Neuron> &neu);
  void ConnectNeighbors();
  void CutLoops();
  void ComputeGravityPoints();
  void ComputeRadius();
};
} // namespace sigen
