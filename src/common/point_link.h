#pragma once
#include "./voxel.h"
#include <vector>
#include <memory>
namespace sigen {
class point_link {
public:
  std::vector<voxel *> voxels_;
  std::vector<point_link *> adjacent_;
  explicit point_link(const std::vector<voxel *> &voxels_);
  bool check_neighbor(const point_link *pl);
  void add_connection(point_link *pl);
};
}
