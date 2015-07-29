#include "./builder.h"
#include <utility>
#include <vector>
#include <queue>
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
    que.push(last);
    while (!que.empty()) {
      neuron_node *cur = que.front();
      que.pop();

    }
  }
  return last;
}
neuron builder::build() {
  neuron ret;
  return ret;
}
} // namespace sigen
