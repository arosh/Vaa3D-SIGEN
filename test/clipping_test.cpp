#include <gtest/gtest.h>
#include <vector>
#include <boost/make_shared.hpp>
#include <set>
#include "sigen/common/neuron.h"
#include "sigen/neuronprocess/neuronprocess.h"
using namespace sigen;
void add(Neuron &n, int id) {
  boost::shared_ptr<NeuronNode> node = boost::make_shared<NeuronNode>();
  node->id_ = id;
  n.storage_.push_back(node);
}

/* l, r : storage_ のインデックス */
void connect(Neuron &n, int l, int r) {
  n.storage_[l]->add_connection(n.storage_[r].get());
  n.storage_[r]->add_connection(n.storage_[l].get());
}

TEST(clipping, clipping) {
  Neuron n;

  for(int i = 0; i < 10; ++i) {
    add(n, i);
  }
  connect(n, 0, 1);
  connect(n, 1, 2);
  connect(n, 2, 3);
  connect(n, 1, 4);
  connect(n, 1, 5);
  connect(n, 1, 6);
  connect(n, 6, 7);
  connect(n, 7, 8);
  connect(n, 8, 9);
  std::vector<Neuron> ns;
  ns.push_back(n);
  std::vector<Neuron> ret = clipping(ns, 2);
  EXPECT_EQ(1, (int)ret.size());
  EXPECT_EQ(1, ret[0].storage_[0]->adjacent_.size());
  EXPECT_EQ(2, ret[0].storage_[1]->adjacent_.size());
}

TEST(clipping, clipping2) {
  Neuron n;

  for(int i = 0; i < 10; ++i) {
    add(n, i);
  }
  connect(n, 0, 1);
  connect(n, 1, 2);
  connect(n, 2, 3);
  connect(n, 1, 4);
  connect(n, 1, 5);
  connect(n, 1, 6);
  connect(n, 6, 7);
  connect(n, 7, 8);
  connect(n, 8, 9);
  std::vector<Neuron> ns;
  ns.push_back(n);
  std::vector<Neuron> ret = clipping(ns, 10);
  EXPECT_EQ(1, (int)ret.size());
  EXPECT_EQ(1, ret[0].storage_[0]->adjacent_.size());
  EXPECT_EQ(2, ret[0].storage_[1]->adjacent_.size());
}
