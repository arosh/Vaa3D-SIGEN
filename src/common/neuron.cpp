#include "./neuron.h"
#include <glog/logging.h>
namespace sigen {
void neuron_part::add_child(neuron_part *child) {
  CHECK(child->parent_ == nullptr);
  child->parent_ = this;
  this->childs_.push_back(child);
}
};
