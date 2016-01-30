#pragma once
#include <vector>
#include "sigen/common/binary_cube.h"
namespace sigen {
namespace interface {
struct Options {
public:
  int volume_threshold;
  double distance_threshold;
  int smoothing_level;
  int clipping_level;
  double scale_xy;
  double scale_z;
};
void Extract(const BinaryCube &cube,
             std::vector<int> &out_n, std::vector<int> &out_type,
             std::vector<double> &out_x, std::vector<double> &out_y, std::vector<double> &out_z,
             std::vector<double> &out_r, std::vector<int> &out_pn, const Options &options);
// void ExtractAndWrite(const BinaryCube &cube, const char *filename, const Options &options);
}; // namespace interface
}; // namespace sigen
