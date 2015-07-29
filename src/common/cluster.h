#pragma once
#include "./voxel.h"
#include <vector>
#include <memory>
namespace sigen {
class cluster {
public:
  std::vector<voxel *> voxels_;
  std::vector<cluster *> adjacent_;
  explicit cluster(const std::vector<voxel *> &voxels_);
  bool check_neighbor(const cluster *pl);
  void add_connection(cluster *pl);
};
}
