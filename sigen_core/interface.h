#pragma once
namespace sigen {
namespace interface {
void run(const int x, const int y, const int z, const bool *data_,
    const double scale_xy, const double scale_z,
    int *out_size, int *out_n, int *out_type,
    double *out_x, double *out_y, double *out_z,
    double *out_r, int *out_pn);
};
};
