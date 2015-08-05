#pragma once
#include "./voxel.h"
#include <vector>
#include <memory>
namespace sigen {
class cluster {
public:
  double gx_, gy_, gz_;
  double radius_;
  std::vector<voxel *> voxels_;
  std::vector<cluster *> adjacent_;
  explicit cluster(const std::vector<voxel *> &voxels);
  bool check_neighbor(const cluster *p);
  void add_connection(cluster *p);
};
} // namespace sigen
