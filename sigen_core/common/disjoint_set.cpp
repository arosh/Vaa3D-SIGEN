#include <algorithm>
#include "common/disjoint_set.h"

namespace sigen {

disjoint_set_internal::disjoint_set_internal(int size) : data(size, -1) {}

int disjoint_set_internal::root(int x) {
  return data[x] < 0 ? x : data[x] = root(data[x]);
}

bool disjoint_set_internal::same(int x, int y) {
  return root(x) == root(y);
}

int disjoint_set_internal::size(int x) {
  return -data[root(x)];
}

void disjoint_set_internal::merge(int x, int y) {
  x = root(x), y = root(y);
  if (x != y) {
    if (size(y) > size(x))
      std::swap(x, y);
    data[x] += data[y];
    data[y] = x;
  }
}

} // namespace sigen
