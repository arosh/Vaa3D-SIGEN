#include "neuron.h"
#include <algorithm>
#include <glog/logging.h>
namespace sigen {
void neuron_node::add_connection(neuron_node *node) {
  DCHECK(std::find(adjacent_.begin(), adjacent_.end(), node) ==
         adjacent_.end());
  adjacent_.push_back(node);
}
};
