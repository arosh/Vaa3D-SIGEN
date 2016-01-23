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
double norm_l2(
    const boost::shared_ptr<NeuronNode> &lhs,
    const boost::shared_ptr<NeuronNode> &rhs) {
  const double dx = std::abs(lhs->gx_ - rhs->gx_);
  const double dy = std::abs(lhs->gy_ - rhs->gy_);
  const double dz = std::abs(lhs->gz_ - rhs->gz_);
  return std::sqrt(dx * dx + dy * dy + dz * dz);
}
// This functions is HOT SPOT.
// This functions is called too many times.
// This is worth to hack.
// Reference
// Golin, M. J., et al.: Simple randomized algorithms for closest pair problems, Nordic Journal of Computing (1995).
std::pair<double, std::pair<int, int> > norm_neuron(const Neuron &lhs, const Neuron &rhs) {
  assert(!lhs.empty());
  assert(!rhs.empty());
  int l = 0;
  int r = 0;
  double minimum = norm_l2(lhs.storage_[0], rhs.storage_[0]);
  for (int i = 0; i < (int)lhs.storage_.size(); ++i) {
    for (int j = 0; j < (int)rhs.storage_.size(); ++j) {
      double d = norm_l2(lhs.storage_[i], rhs.storage_[j]);
      if (minimum > d) {
        minimum = d;
        l = i;
        r = j;
      }
    }
  }
  return std::make_pair(minimum, std::make_pair(l, r));
}
std::vector<Neuron> interpolate(const std::vector<Neuron> &input, const double dt, const int vt) {
  const int N = input.size();
  std::vector<Neuron> forest;
  for (int i = 0; i < N; ++i) {
    forest.push_back(input[i].clone());
  }
  DisjointSet<int> set;
  std::vector<bool> is_not_small(forest.size(), false);
  for (int i = 0; i < N; ++i) {
    if ((int)forest[i].storage_.size() >= vt) {
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
  for (int i = 0; i < N; ++i) {
    if (is_not_small[i]) {
      for (int j = i + 1; j < N; ++j) {
        if (is_not_small[j]) {
          assert(i != j);
          double d = norm_neuron(forest[i], forest[j]).first;
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
    if (forest[l].empty())
      continue;
    if (forest[r].empty())
      continue;
    if (set.same(l, r))
      continue;
    // if(forest[l].storage_.size() < forest[r].storage_.size()) std::swap(l, r);
    std::pair<double, std::pair<int, int> > dist = norm_neuron(forest[l], forest[r]);
    set.merge(l, r);
    forest[l].storage_[dist.second.first]->add_connection(forest[r].storage_[dist.second.second].get());
    forest[r].storage_[dist.second.second]->add_connection(forest[l].storage_[dist.second.first].get());
    forest[l].extend(forest[r]);
    forest[r].clear();

    for (int i = 0; i < N; ++i) {
      if (i != l && is_not_small[i] && forest[i].empty() == false) {
        assert(l != i);
        double d = norm_neuron(forest[l], forest[i]).first;
        if (d <= dt) {
          pq.push(std::make_pair(d, std::make_pair(l, i)));
        }
      }
    }
  }
  for (int i = 0; i < (int)forest.size(); ++i) {
    if (forest[i].empty()) {
      forest.erase(forest.begin() + i);
      i--;
    }
  }
  return forest;
}

struct point_and_radius {
  double gx_, gy_, gz_, radius_;
  inline void coord(const double gx, const double gy, const double gz) {
    gx_ = gx;
    gy_ = gy;
    gz_ = gz;
  }
};
std::vector<Neuron> smoothing(const std::vector<Neuron> &input, const int n_iter) {
  std::vector<Neuron> forest;
  for (int i = 0; i < (int)input.size(); ++i) {
    forest.push_back(input[i].clone());
  }
  for (int iter = 0; iter < n_iter; ++iter) {
    std::map<int, point_and_radius> next_value;
    for (int i = 0; i < (int)forest.size(); ++i) {
      for (boost::shared_ptr<NeuronNode> node : forest[i].storage_) {
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
        point_and_radius next_node;
        next_node.coord(Mean(gx), Mean(gy), Mean(gz));
        next_node.radius_ = Mean(radius);
        next_value[node->id_] = next_node;
      }
    }
    for (int i = 0; i < (int)forest.size(); ++i) {
      for (boost::shared_ptr<NeuronNode> node : forest[i].storage_) {
        point_and_radius next_node = next_value[node->id_];
        node->coord(next_node.gx_, next_node.gy_, next_node.gz_);
        node->radius_ = next_node.radius_;
      }
    }
  }
  return forest;
}
int clipping_dfs(
    NeuronNode *node,
    NeuronNode *parent,
    const int level,
    std::set<int> &will_remove) {
  int mindepth = std::numeric_limits<int>::max();
  int longpath = 0;
  for (NeuronNode *next : node->adjacent_) {
    if (next != parent) {
      int dep = clipping_dfs(next, node, level, will_remove);
      if (dep > level)
        longpath++;
      mindepth = std::min(mindepth, dep);
    }
  }
  if (longpath > 0) {
    for (NeuronNode *next : node->adjacent_) {
      if (next != parent) {
        int dep = clipping_dfs(next, node, level, will_remove);
        if (dep <= level) {
          will_remove.insert(next->id_);
        }
      }
    }
  } else {
    int longest_depth = 0;
    NeuronNode *longest_child = NULL;
    for (NeuronNode *next : node->adjacent_) {
      if (next != parent) {
        int dep = clipping_dfs(next, node, level, will_remove);
        if (longest_depth < dep) {
          longest_depth = dep;
          longest_child = next;
        }
      }
    }
    if (longest_depth > 0) {
      for (NeuronNode *next : node->adjacent_) {
        if (next != parent) {
          if (next != longest_child) {
            will_remove.insert(next->id_);
          }
        }
      }
    }
  }
  if (mindepth == std::numeric_limits<int>::max()) {
    mindepth = 0;
  }
  return mindepth + 1;
}
std::vector<Neuron> clipping(const std::vector<Neuron> &input, const int level) {
  std::set<int> will_remove;
  std::vector<Neuron> forest;
  for (int i = 0; i < (int)input.size(); ++i) {
    forest.push_back(input[i].clone());
    // infinite loop?
    clipping_dfs(forest[i].root_, NULL, level, will_remove);
  }
  for (int i = 0; i < (int)forest.size(); ++i) {
    for (int j = 0; j < (int)forest[i].storage_.size(); ++j) {
      forest[i].storage_[j]->remove_connection(will_remove);
    }
  }
  return forest;
}
}; // namespace sigen
