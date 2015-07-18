#include "neuron.h"
#include <glog/logging.h>
namespace sigen {
void neuron::add_child(neuron *child) {
  CHECK(child->parent_ == nullptr);
  child->parent_ = this;
  this->childs_.push_back(child);
}
};
