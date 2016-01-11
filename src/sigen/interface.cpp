#include <cassert>
#include <iostream>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "sigen/interface.h"
#include "sigen/common/binary_cube.h"
#include "sigen/extractor/extractor.h"
#include "sigen/builder/builder.h"
#include "sigen/neuronprocess/neuronprocess.h"

#define DEBUG(x) std::cerr << #x << " = " << x << std::endl

namespace sigen {
namespace interface {
static void write(
    const NeuronNode *cur, const int parent_id,
    std::vector<int> &out_n, std::vector<int> &out_type,
    std::vector<double> &out_x, std::vector<double> &out_y, std::vector<double> &out_z,
    std::vector<double> &out_r, std::vector<int> &out_pn) {
  int type_id = -1;
  switch (cur->type_) {
  case neuron_type::EDGE:
    type_id = 6;
    break;
  case neuron_type::BRANCH:
    type_id = 5;
    break;
  case neuron_type::CONNECT:
    type_id = 3;
    break;
  }

  assert(type_id != -1);

  out_n.push_back(cur->id_);
  out_type.push_back(type_id);
  out_x.push_back(cur->gx_);
  out_y.push_back(cur->gy_);
  out_z.push_back(cur->gz_);
  out_r.push_back(cur->radius_);
  out_pn.push_back(parent_id);

  for (const NeuronNode *next : cur->adjacent_) {
    if (next->id_ != parent_id) {
      write(next, cur->id_, out_n, out_type, out_x, out_y, out_z, out_r, out_pn);
    }
  }
}

void run(
    const BinaryCube &cube,
    const double scale_xy, const double scale_z,
    std::vector<int> &out_n, std::vector<int> &out_type,
    std::vector<double> &out_x, std::vector<double> &out_y, std::vector<double> &out_z,
    std::vector<double> &out_r, std::vector<int> &out_pn, Options options) {
  sigen::Extractor ext(cube);
  std::vector<boost::shared_ptr<sigen::Cluster> > clusters = ext.extract();
  sigen::Builder bld(clusters, scale_xy, scale_z);
  std::vector<sigen::Neuron> neurons = bld.build();
  // std::cerr << "build finished" << std::endl;
  neurons = interpolate(neurons, options.distance_threshold, options.volume_threshold);
  // std::cerr << "interpolate finished" << std::endl;
  neurons = smoothing(neurons, options.smoothing_level);
  // std::cerr << "smoothing finished" << std::endl;
  // neurons = clipping(neurons, options.clipping_level);
  // std::cerr << "clipping finished" << std::endl;

  for (int i = 0; i < (int)neurons.size(); ++i) {
    write(neurons[i].root_, -1, out_n, out_type, out_x, out_y, out_z, out_r, out_pn);
  }
}
}; // namespace interface
}; // namespace sigen
