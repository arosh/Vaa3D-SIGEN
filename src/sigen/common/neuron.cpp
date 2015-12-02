#include <cassert>
#include <algorithm>
#include "sigen/common/neuron.h"
namespace sigen {
void neuron_node::add_connection(neuron_node *node) {
  assert(std::find(adjacent_.begin(), adjacent_.end(), node) == adjacent_.end());
  adjacent_.push_back(node);
}
}; // namespace sigen
