#include <cmath>
#include <queue>
#include <utility>
#include <set>
#include <map>
#include <limits>
#include <algorithm>
#include <vector>
#include <iostream>
#include <functional>
#include "sigen/neuronprocess/neuronprocess.h"
#include "sigen/common/disjoint_set.h"
#include "sigen/common/math.h"
namespace sigen {
static double normL2(
    const NeuronNodePtr &lhs,
    const NeuronNodePtr &rhs) {
  const double dx = std::abs(lhs->gx_ - rhs->gx_);
  const double dy = std::abs(lhs->gy_ - rhs->gy_);
  const double dz = std::abs(lhs->gz_ - rhs->gz_);
  return std::sqrt(dx * dx + dy * dy + dz * dz);
}

// This function is HOT SPOT.
// This function spent too much time.
// There is worth to fix. Randomized algorithms by [Golin 1995] provides O(N) time.
//
// Reference
// ---------
// Golin, M. J., et al.: Simple randomized algorithms for closest pair problems, Nordic Journal of Computing (1995).
static std::pair<double, std::pair<int, int> > normNeuron(const Neuron &lhs, const Neuron &rhs) {
  assert(!lhs.IsEmpty());
  assert(!rhs.IsEmpty());
  int l = 0;
  int r = 0;
  double minimum = normL2(lhs.storage_[0], rhs.storage_[0]);
  for (int i = 0; i < (int)lhs.storage_.size(); ++i) {
    for (int j = 0; j < (int)rhs.storage_.size(); ++j) {
      double d = normL2(lhs.storage_[i], rhs.storage_[j]);
      if (minimum > d) {
        minimum = d;
        l = i;
        r = j;
      }
    }
  }
  return std::make_pair(minimum, std::make_pair(l, r));
}

std::vector<Neuron> Interpolate(const std::vector<Neuron> &input, const double dt, const int vt) {
  const int N = input.size();
  std::vector<Neuron> forest;
  for (int i = 0; i < N; ++i) {
    forest.push_back(input[i].Clone());
  }
  DisjointSet<int> set;
  std::vector<bool> is_not_small(forest.size(), false);
  for (int i = 0; i < N; ++i) {
    if ((int)forest[i].storage_.size() >= vt) {
      set.Add(i);
      is_not_small[i] = true;
    }
  }
  set.SetUp();
  typedef std::pair<double, std::pair<int, int> > priority_queue_node;
  std::priority_queue<
      priority_queue_node,
      std::vector<priority_queue_node>,
      std::greater<priority_queue_node> > pq;
  for (int i = 0; i < N; ++i) {
    if (is_not_small[i]) {
      for (int j = i + 1; j < N; ++j) {
        if (is_not_small[j]) {
          assert(i != j);
          double d = normNeuron(forest[i], forest[j]).first;
          if (d <= dt) {
            pq.push(std::make_pair(d, std::make_pair(i, j)));
          }
        }
      }
    }
  }
  while (!pq.empty()) {
    priority_queue_node node = pq.top();
    pq.pop();
    int l = node.second.first;
    int r = node.second.second;
    if (forest[l].IsEmpty())
      continue;
    if (forest[r].IsEmpty())
      continue;
    if (set.IsSame(l, r))
      continue;
    // if(forest[l].storage_.size() < forest[r].storage_.size()) std::swap(l, r);
    std::pair<double, std::pair<int, int> > dist = normNeuron(forest[l], forest[r]);
    set.Merge(l, r);
    forest[l].storage_[dist.second.first]->AddConnection(forest[r].storage_[dist.second.second].get());
    forest[r].storage_[dist.second.second]->AddConnection(forest[l].storage_[dist.second.first].get());
    forest[l].Extend(forest[r]);
    forest[r].Clear();

    for (int i = 0; i < N; ++i) {
      if (i != l && is_not_small[i] && forest[i].IsEmpty() == false) {
        assert(l != i);
        double d = normNeuron(forest[l], forest[i]).first;
        if (d <= dt) {
          pq.push(std::make_pair(d, std::make_pair(l, i)));
        }
      }
    }
  }
  for (int i = 0; i < (int)forest.size(); ++i) {
    if (forest[i].IsEmpty()) {
      forest.erase(forest.begin() + i);
      i--;
    }
  }
  return forest;
}

struct PointAndRadius {
  double gx_, gy_, gz_, radius_;
  inline void setCoord(const double gx, const double gy, const double gz) {
    gx_ = gx;
    gy_ = gy;
    gz_ = gz;
  }
};

std::vector<Neuron> Smoothing(const std::vector<Neuron> &input, const int n_iter) {
  std::vector<Neuron> forest;
  for (int i = 0; i < (int)input.size(); ++i) {
    forest.push_back(input[i].Clone());
  }
  for (int iter = 0; iter < n_iter; ++iter) {
    std::map<int, PointAndRadius> next_value;
    for (int i = 0; i < (int)forest.size(); ++i) {
      for (NeuronNodePtr node : forest[i].storage_) {
        std::vector<double> gx, gy, gz, radius;
        gx.push_back(node->gx_);
        gy.push_back(node->gy_);
        gz.push_back(node->gz_);
        radius.push_back(node->radius_);
        for (NeuronNode *adj : node->adjacent_) {
          gx.push_back(adj->gx_);
          gy.push_back(adj->gy_);
          gz.push_back(adj->gz_);
          radius.push_back(adj->radius_);
        }
        PointAndRadius next_node;
        next_node.setCoord(Mean(gx), Mean(gy), Mean(gz));
        next_node.radius_ = Mean(radius);
        next_value[node->id_] = next_node;
      }
    }
    for (int i = 0; i < (int)forest.size(); ++i) {
      for (NeuronNodePtr node : forest[i].storage_) {
        PointAndRadius next_node = next_value[node->id_];
        node->setCoord(next_node.gx_, next_node.gy_, next_node.gz_);
        node->radius_ = next_node.radius_;
      }
    }
  }
  return forest;
}

// return max_height
static int clippingDfs(
    NeuronNode *node,
    NeuronNode *parent,
    const int level,
    std::set<int> &will_remove,
    std::map<NeuronNode *, int> &memo) {
  if (memo.count(node))
    return memo[node];
  if (node->CountNumChild(parent) < 2) {
    // If count_num_child == 1
    for (NeuronNode *next : node->adjacent_) {
      if (next != parent) {
        return memo[node] = clippingDfs(next, node, level, will_remove, memo) + 1;
      }
    }
    // If count_num_child == 0
    return 1;
  }
  int has_longpath = 0;
  for (NeuronNode *next : node->adjacent_) {
    if (next != parent) {
      int depth = clippingDfs(next, node, level, will_remove, memo);
      if (depth > level)
        has_longpath = true;
    }
  }
  if (has_longpath) {
    int maxdepth = 0;
    for (NeuronNode *next : node->adjacent_) {
      if (next != parent) {
        int depth = clippingDfs(next, node, level, will_remove, memo);
        if (depth <= level) {
          will_remove.insert(next->id_);
        }
        maxdepth = std::max(maxdepth, depth);
      }
    }
    return memo[node] = maxdepth + 1;
  } else {
    int maxdepth = 0;
    NeuronNode *longest_child = NULL;
    for (NeuronNode *next : node->adjacent_) {
      if (next != parent) {
        int depth = clippingDfs(next, node, level, will_remove, memo);
        if (maxdepth < depth) {
          maxdepth = depth;
          longest_child = next;
        }
      }
    }
    if (maxdepth > 0) {
      for (NeuronNode *next : node->adjacent_) {
        if (next != parent) {
          if (next != longest_child) {
            will_remove.insert(next->id_);
          }
        }
      }
    }
    return memo[node] = maxdepth + 1;
  }
}

std::vector<Neuron> Clipping(const std::vector<Neuron> &input, const int level) {
  std::set<int> will_remove;
  std::vector<Neuron> forest;
  std::map<NeuronNode *, int> memo;
  for (int i = 0; i < (int)input.size(); ++i) {
    forest.push_back(input[i].Clone());
    clippingDfs(forest[i].root_, NULL, level, will_remove, memo);
  }
  for (int i = 0; i < (int)forest.size(); ++i) {
    for (int j = 0; j < (int)forest[i].storage_.size(); ++j) {
      forest[i].storage_[j]->RemoveConnection(will_remove);
    }
  }
  return forest;
}
}; // namespace sigen
