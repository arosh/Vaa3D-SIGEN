#include <cmath>
#include <queue>
#include <utility>
#include "sigen/neuronprocess/neuronprocess.h"
#include "sigen/common/disjoint_set.h"
namespace sigen {
double norm_l2(
    const boost::shared_ptr<neuron_node> &lhs,
    const boost::shared_ptr<neuron_node> &rhs) {
  const double dx = std::abs(lhs->gx_ - rhs->gx_);
  const double dy = std::abs(lhs->gy_ - rhs->gy_);
  const double dz = std::abs(lhs->gz_ - rhs->gz_);
  return std::sqrt(dx*dx + dy*dy + dz*dz);
}
std::pair<double, std::pair<int, int> > norm_neuron(const neuron &lhs, const neuron &rhs) {
  assert(lhs.storage_.size() > 0);
  assert(rhs.storage_.size() > 0);
  int l = 0;
  int r = 0;
  double minimum = norm_l2(lhs.storage_[0], rhs.storage_[0]);
  for(int i = 0; i < (int)lhs.storage_.size(); ++i) {
    for(int j = 0; j < (int)rhs.storage_.size(); ++j) {
      double d = norm_l2(lhs.storage_[i], rhs.storage_[j]);
      if(minimum > d) {
        minimum = d;
        l = i;
        r = j;
      }
    }
  }
  return std::make_pair(minimum, std::make_pair(l, r));
}
std::vector<neuron> interpolate(const std::vector<neuron> &input, const double dt, const int vt) {
  const int N = input.size();
  std::vector<neuron> forest;
  for(int i = 0; i < N; ++i) {
    forest.push_back(input[i].clone());
  }
  disjoint_set<int> set;
  std::vector<bool> is_not_small(forest.size(), false);
  for(int i = 0; i < N; ++i) {
    if((int)forest[i].storage_.size() >= vt) {
      set.add(i);
      is_not_small[i] = true;
    }
  }
  set.setup();
  typedef std::pair<double, std::pair<int, int> > priority_queue_node;
  std::priority_queue<
    priority_queue_node,
    std::vector<priority_queue_node>,
    std::greater<priority_queue_node> > pq;
  for(int i = 0; i < N; ++i) {
    if(is_not_small[i]) {
      for(int j = i + 1; j < N; ++j) {
        if(is_not_small[j]) {
          assert(i != j);
          double d = norm_neuron(forest[i], forest[j]).first;
          if(d <= dt) {
            pq.push(std::make_pair(d, std::make_pair(i, j)));
          }
        }
      }
    }
  }
  while(!pq.empty()) {
    priority_queue_node node = pq.top();
    pq.pop();
    int l = node.second.first;
    int r = node.second.second;
    if(forest[l].storage_.size() == 0) continue;
    if(forest[r].storage_.size() == 0) continue;
    if(set.same(l, r)) continue;
    // if(forest[l].storage_.size() < forest[r].storage_.size()) std::swap(l, r);
    std::pair<double, std::pair<int, int> > dist = norm_neuron(forest[l], forest[r]);
    set.merge(l, r);
    forest[l].storage_[dist.second.first]->add_connection(forest[r].storage_[dist.second.second].get());
    forest[r].storage_[dist.second.second]->add_connection(forest[l].storage_[dist.second.first].get());
    forest[l].storage_.insert(
        forest[l].storage_.end(),
        forest[r].storage_.begin(),
        forest[r].storage_.end());
    forest[r].storage_.clear();

    for(int i = 0; i < N; ++i) {
      if(i != l && is_not_small[i] && forest[i].storage_.size() != 0) {
        assert(l != i);
        double d = norm_neuron(forest[l], forest[i]).first;
        if(d <= dt) {
          pq.push(std::make_pair(d, std::make_pair(l, i)));
        }
      }
    }
  }
  for(int i = 0; i < (int)forest.size(); ++i) {
    if(forest[i].storage_.size() == 0) {
      forest.erase(forest.begin() + i);
      i--;
    }
  }
  return forest;
}
};
