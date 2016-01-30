#include <cassert>
#include <iostream>
#include <vector>
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

void Extract(
    const BinaryCube &cube,
    std::vector<int> &out_n, std::vector<int> &out_type,
    std::vector<double> &out_x, std::vector<double> &out_y, std::vector<double> &out_z,
    std::vector<double> &out_r, std::vector<int> &out_pn,
    const Options &options) {
  bool print_progress = true;
  sigen::Extractor ext(cube);
  if (print_progress)
    std::cerr << "extract start" << std::endl;
  std::vector<ClusterPtr> clusters = ext.Extract();
  if (print_progress)
    std::cerr << "extract finished" << std::endl;
  sigen::Builder bld(clusters, options.scale_xy, options.scale_z);
  std::vector<sigen::Neuron> neurons = bld.Build();
  if (print_progress)
    std::cerr << "build finished" << std::endl;
  neurons = Interpolate(neurons, options.distance_threshold, options.volume_threshold);
  if (print_progress)
    std::cerr << "interpolate finished" << std::endl;
  neurons = Smoothing(neurons, options.smoothing_level);
  if (print_progress)
    std::cerr << "smoothing finished" << std::endl;
  neurons = Clipping(neurons, options.clipping_level);
  if (print_progress)
    std::cerr << "clipping finished" << std::endl;

  for (int i = 0; i < (int)neurons.size(); ++i) {
    write(neurons[i].root_, -1, out_n, out_type, out_x, out_y, out_z, out_r, out_pn);
  }
}
void ExtractAndWrite(const BinaryCube &cube, const char *filename, const Options &options) {
  // not implemented
  assert(false);
}
}; // namespace interface
}; // namespace sigen
