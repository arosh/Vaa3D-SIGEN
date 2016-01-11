#include <algorithm>
#include "sigen/common/disjoint_set.h"

namespace sigen {

DisjointSetInternal::DisjointSetInternal(int size) : data(size, -1) {}

int DisjointSetInternal::root(int x) {
  return data[x] < 0 ? x : data[x] = root(data[x]);
}

bool DisjointSetInternal::same(int x, int y) {
  return root(x) == root(y);
}

int DisjointSetInternal::size(int x) {
  return -data[root(x)];
}

void DisjointSetInternal::merge(int x, int y) {
  x = root(x), y = root(y);
  if (x != y) {
    if (size(y) > size(x))
      std::swap(x, y);
    data[x] += data[y];
    data[y] = x;
  }
}

} // namespace sigen
