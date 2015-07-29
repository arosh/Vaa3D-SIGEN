#include "./neuron.h"
#include <algorithm>
#include <glog/logging.h>
namespace sigen {
void neuron_node::add_child(neuron_node *node) {
  CHECK(node->parent_ == nullptr);
  DCHECK(std::find(adjacent_.begin(), adjacent_.end(), node) == adjacent_.end());
  DCHECK(std::find(node->adjacent_.begin(), node->adjacent_.end(), this) == node->adjacent_.end());
  adjacent_.push_back(node);
  node->adjacent_.push_back(this);
  node->parent_ = this;
}
};
