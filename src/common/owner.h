#pragma once
#include <memory>
#include <vector>
template <class T>
class owner {
  std::vector<std::shared_ptr<T>> data_;
public:
  std::shared_ptr<T> add(std::shared_ptr<T> ptr) {
    data_.push_back(ptr);
    return ptr;
  }
  void clear() {
    data_.clear();
  }
};
