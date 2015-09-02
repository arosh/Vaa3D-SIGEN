#pragma once
#include <tuple>
namespace sigen {
template <class T>
class point {
public:
  T x_, y_, z_;
  point(T x, T y, T z) : x_(x), y_(y), z_(z) {}
  bool operator<(const point<T> &other) const {
    return std::tie(x_, y_, z_) < std::tie(other.x_, other.y_, other.z_);
  }
};
} // namespace sigen
