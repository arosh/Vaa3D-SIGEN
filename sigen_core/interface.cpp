#include <cassert>
#include "interface.h"
#include "common/binary_cube.h"
#include "extractor/extractor.h"
#include "builder/builder.h"

namespace sigen {
namespace interface {
static void write(
    const neuron_node *cur, const int parent_id,
    int &row, int *out_n, int *out_type,
    double *out_x, double *out_y, double *out_z,
    double *out_r, int *out_pn) {
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

  out_n[row] = cur->id_;
  out_type[row] = type_id;
  out_x[row] = cur->gx_;
  out_y[row] = cur->gy_;
  out_z[row] = cur->gz_;
  out_r[row] = cur->radius_;
  out_pn[row] = parent_id;
  ++row;

  for(const neuron_node *next : cur->adjacent_) {
    if (next->id_ != parent_id) {
      write(next, cur->id_, row, out_n, out_type, out_x, out_y, out_z, out_r, out_pn);
    }
  }
}

void run(
    const int x, const int y, const int z, const bool *data_,
    const double scale_xy, const double scale_z,
    int *out_size, int *out_n, int *out_type,
    double *out_x, double *out_y, double *out_z,
    double *out_r, int *out_pn) {
  binary_cube cube(x, y, z);
  const int sx = 1;
  const int sy = x;
  const int sz = x * y;
  for(int i = 0; i < x; ++i) {
    for(int j = 0; j < y; ++j) {
      for(int k = 0; k < z; ++k) {
        cube[i][j][k] = data_[sx * i + sy * j + sz * k];
      }
    }
  }

  sigen::extractor ext(cube);
  std::vector<std::shared_ptr<sigen::cluster> > clusters = ext.extract();
  sigen::builder bld(clusters, scale_xy, scale_z);
  std::vector<sigen::neuron> neurons = bld.build();

  int count = 0;
  for(int i = 0; i < (int)neurons.size(); ++i) {
    count += neurons[i].storage_.size();
  }
  *out_size = count;
  out_n = new int[count];
  out_type = new int[count];
  out_x = new double[count];
  out_y = new double[count];
  out_z = new double[count];
  out_r = new double[count];
  out_pn = new int[count];

  int row = 0;
  for(int i = 0; i < (int)neurons.size(); ++i) {
    write(neurons[i].root_, -1, row, out_n, out_type, out_x, out_y, out_z, out_r, out_pn);
  }
}
};
};
