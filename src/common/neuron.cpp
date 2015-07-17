#include "neuron.h"
#include <glog/logging.h>
namespace sigen {
void neuron::connect(Neuron parent, Neuron child) {
  CHECK(child->parent_.expired());
  child->parent_ = parent;
  parent->childs_.push_back(child);
}
};
