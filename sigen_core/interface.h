#pragma once
#include "common/binary_cube.h"
namespace sigen {
namespace interface {
void run(const binary_cube &cube,
         const double scale_xy, const double scale_z,
         std::vector<int> &out_n, std::vector<int> &out_type,
         std::vector<double> &out_x, std::vector<double> &out_y, std::vector<double> &out_z,
         std::vector<double> &out_r, std::vector<int> &out_pn);
};
};
