#pragma once
#include <vector>
#include <boost/utility.hpp>
#include <boost/shared_ptr.hpp>
namespace sigen {
class Voxel;
typedef boost::shared_ptr<Voxel> VoxelPtr;
class Voxel : boost::noncopyable {
public:
  int x_, y_, z_;
  bool flag_; // any functions can use this variable
  int label_; // any functions can use this variable
  std::vector<Voxel *> adjacent_;

  Voxel(int x, int y, int z)
      : x_(x), y_(y), z_(z), flag_(false), label_(-1) {}

  inline void AddConection(Voxel *p) {
    assert(std::find(adjacent_.begin(), adjacent_.end(), p) == adjacent_.end());
    adjacent_.push_back(p);
  }
  inline void AddConection(VoxelPtr p) {
    this->AddConection(p.get());
  }
};
} // namespace sigen
