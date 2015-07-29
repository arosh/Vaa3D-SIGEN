#include "./builder.h"
#include <utility>
#include <vector>
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
neuron_node *builder::find_edge() {
}
std::pair<std::vector<std::shared_ptr<neuron_node>>, neuron_node *> builder::build() {
  std::pair<std::vector<std::shared_ptr<neuron_node>>, neuron_node *> ret;
  return ret;
}
} // namespace sigen
