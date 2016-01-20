#include <cassert>
#include <algorithm>
#include <map>
#include "sigen/common/neuron.h"
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
namespace sigen {
void NeuronNode::add_connection(NeuronNode *node) {
  assert(!adjacent_.count(node));
  adjacent_.insert(node);
}
void NeuronNode::remove_connection(NeuronNode *node) {
  assert(adjacent_.count(node));
  adjacent_.erase(node);
}
void NeuronNode::remove_connection(const std::set<int> &nodes) {
  // `erase` invalidates iterator. Be careful.
  // http://qiita.com/satoruhiga/items/fa6eae09c9d89bd48b5d
  std::set<NeuronNode *>::iterator it = adjacent_.begin();
  while (it != adjacent_.end()) {
    if (nodes.count((*it)->id_)) {
      adjacent_.erase(it++);
    } else {
      ++it;
    }
  }
}
Neuron Neuron::clone() const {
  assert(this->root_ != NULL);
  Neuron ret;
  std::map<NeuronNode *, int> ptr_to_index;
  for (int i = 0; i < (int)this->storage_.size(); ++i) {
    boost::shared_ptr<NeuronNode> ptr = this->storage_[i];
    ptr_to_index[ptr.get()] = i;
    ret.storage_.push_back(ptr->clone());
  }

  for (int i = 0; i < (int)this->storage_.size(); ++i) {
    for (NeuronNode *adj : this->storage_[i].get()->adjacent_) {
      NeuronNode *p = ret.storage_[ptr_to_index[adj]].get();
      ret.storage_[i]->add_connection(p);
    }
  }

  ret.setRoot(ptr_to_index[this->root_]);
  return ret;
}
}; // namespace sigen
