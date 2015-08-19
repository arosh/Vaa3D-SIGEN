#pragma once
namespace sigen {
template <class T>
class point {
public:
  T x_, y_, z_;
  point(T x, T y, T z) : x_(x), y_(y), z_(z) {}
};
}
