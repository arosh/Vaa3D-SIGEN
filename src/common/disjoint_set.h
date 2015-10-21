// https://www.topcoder.com/community/data-science/data-science-tutorials/disjoint-set-data-structures/
#pragma once
#include <vector>
#include <map>
#include <memory>
#include <boost/utility.hpp>
#include <glog/logging.h>
namespace sigen {
class disjoint_set_internal {
  std::vector<int> data;
  int root(int x);

public:
  disjoint_set_internal(int size);
  int size(int x);
  bool same(int x, int y);
  void merge(int x, int y);
};
template <class T>
class disjoint_set : boost::noncopyable {
  std::map<T, int> forward;
  std::shared_ptr<disjoint_set_internal> U;

public:
  void add(T x) {
    int next_id = forward.size();
    forward[x] = next_id;
  }
  void setup() {
    U = std::make_shared<disjoint_set_internal>(forward.size());
  }
  int size(T x) {
    DCHECK((bool)U);
    return U->size(forward[x]);
  }
  bool same(T x, T y) {
    DCHECK((bool)U);
    return U->same(forward[x], forward[y]);
  }
  void merge(T x, T y) {
    DCHECK((bool)U);
    U->merge(forward[x], forward[y]);
  }
};
}
