#pragma once
#include <vector>
#include <boost/utility.hpp>
namespace sigen {
class Voxel : boost::noncopyable {
public:
  int x_, y_, z_;
  bool flag_; // any functions can use this variable
  int label_; // any functions can use this variable
  std::vector<Voxel *> adjacent_;
  Voxel(int x, int y, int z);
  void add_connection(Voxel *p);
};
} // namespace sigen
