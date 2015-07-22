#include "builder.h"
namespace sigen {
builder::builder(const std::vector<std::shared_ptr<point_link>> &data)
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
  // pixel resolution???
}
neuron_part *builder::find_edge() {
}
std::pair<std::vector<std::shared_ptr<neuron_part>>, neuron_part *> builder::build() {
  std::pair<std::vector<std::shared_ptr<neuron_part>>, neuron_part *> ret;
  return ret;
}
};
