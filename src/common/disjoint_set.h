// see https://www.topcoder.com/community/data-science/data-science-tutorials/disjoint-set-data-structures/
#pragma once
#include <vector>
namespace sigen {
class disjoint_set_internal {
  std::vector<int> data;
public:
  disjoint_set_internal(int size);
  int root(int x);
  bool same(int x, int y);
  int size(int x);
  void merge(int x, int y);
};
}
