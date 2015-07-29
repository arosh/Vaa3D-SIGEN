#include "./builder.h"
#include <utility>
#include <vector>
#include <queue>
#include <set>
namespace sigen {
builder::builder(const std::vector<std::shared_ptr<cluster>> &data)
    : data_(data) {}
void builder::connect_neighbor() {
  for (int i = 0; i < (int)data_.size(); ++i) {
    for (int j = i + 1; j < (int)data_.size(); ++j) {
      if (data_[i]->check_neighbor(data_[j].get())) {
        data_[i]->add_connection(data_[j].get());
        data_[j]->add_connection(data_[i].get());
      }
    }
  }
}
void builder::connect_interpolate(double dt) {
  // 重くなりそうなので，重心の距離で
  // pixel resolution???
}
static neuron_node *find_edge(neuron_node *node) {
  std::queue<neuron_node *> que;
  neuron_node *last = node;
  for (int i = 0; i < 2; ++i) {
    std::set<neuron_node *> used;
    que.push(last);
    used.insert(last);
    while (!que.empty()) {
      neuron_node *cur = que.front();
      que.pop();
      for(neuron_node *next : node->adjacent_) {
        if(used.count(next)) continue;
        que.push(next);
        used.insert(next);
      }
    }
  }
  return last;
}
neuron builder::build() {
  neuron ret;
  return ret;
}
} // namespace sigen
