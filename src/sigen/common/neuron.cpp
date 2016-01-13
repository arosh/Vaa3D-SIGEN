#include <cassert>
#include <algorithm>
#include <map>
#include "sigen/common/neuron.h"
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
namespace sigen {
void NeuronNode::add_connection(NeuronNode *node) {
  assert(std::find(adjacent_.begin(), adjacent_.end(), node) == adjacent_.end());
  adjacent_.push_back(node);
}
Neuron Neuron::clone() const {
  Neuron ret;
  std::map<NeuronNode *, int> ptr_to_int;
  for (int i = 0; i < (int)this->storage_.size(); ++i) {
    boost::shared_ptr<NeuronNode> ptr = this->storage_[i];
    ptr_to_int[ptr.get()] = i;

    boost::shared_ptr<NeuronNode> node = boost::make_shared<NeuronNode>();
    node->id_ = ptr->id_;
    node->gx_ = ptr->gx_;
    node->gy_ = ptr->gy_;
    node->gz_ = ptr->gz_;
    node->radius_ = ptr->radius_;
    node->type_ = ptr->type_;
    ret.storage_.push_back(node);
  }

  for (int i = 0; i < (int)this->storage_.size(); ++i) {
    for (NeuronNode *adj : this->storage_[i].get()->adjacent_) {
      NeuronNode *p = ret.storage_[ptr_to_int[adj]].get();
      ret.storage_[i]->add_connection(p);
    }
  }

  ret.root_ = ret.storage_[ptr_to_int[this->root_]].get();
  return ret;
}
}; // namespace sigen
