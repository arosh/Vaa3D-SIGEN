#include "./neuron.h"
#include <glog/logging.h>
namespace sigen {
void neuron_node::add_child(neuron_node *child) {
  CHECK(child->parent_ == nullptr);
  child->parent_ = this;
  this->childs_.push_back(child);
}
};
