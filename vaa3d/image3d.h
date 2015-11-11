#pragma once
class image3d {
public:
  int x_, y_, z_;
  // Do not use std::vector (it may cause ABI problem).
  // bool[][][]
  bool *data_;
  image3d(int x, int y, int z);
  virtual ~image3d();
  bool &operator()(int x, int y, int z);
  const bool &operator()(int x, int y, int z) const;
};
