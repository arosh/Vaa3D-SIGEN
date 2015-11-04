#pragma once
#include <vector>
#include "noncopyable.h"
namespace sigen {
class voxel : noncopyable {
public:
  int x_, y_, z_;
  bool flag_; // any functions can use this variable
  int label_; // any functions can use this variable
  std::vector<voxel *> adjacent_;
  voxel(int x, int y, int z);
  void add_connection(voxel *p);
};
} // namespace sigen
