#pragma once
#include <vector>
#include "sigen/common/binary_cube.h"
namespace sigen {
namespace interface {
struct Options {
  int volume_threshold;
  double distance_threshold;
  int smoothing_level;
  int clipping_level;
};
void run(const BinaryCube &cube,
         const double scale_xy, const double scale_z,
         std::vector<int> &out_n, std::vector<int> &out_type,
         std::vector<double> &out_x, std::vector<double> &out_y, std::vector<double> &out_z,
         std::vector<double> &out_r, std::vector<int> &out_pn, Options options);
};
};
