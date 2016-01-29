#include "sigen/builder/builder.h"
#include "sigen/common/disjoint_set.h"
#include <cassert>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <utility>
#include <vector>
#include <queue>
#include <stack>
#include <set>
#include <map>
#include <boost/foreach.hpp>
namespace sigen {

Builder::Builder(const std::vector<ClusterPtr> &data,
                 const double scale_xy, const double scale_z)
    : is_radius_computed_(false), data_(data), scale_xy_(scale_xy), scale_z_(scale_z) {}

void Builder::ConnectNeighbors() {
  std::multimap<IPoint, int> coord_to_index;
  for (int i = 0; i < (int)data_.size(); ++i) {
    BOOST_FOREACH (const IPoint &p, data_[i]->points_) {
      coord_to_index.insert(std::make_pair(p, i));
    }
  }
  for (int i = 0; i < (int)data_.size(); ++i) {
    BOOST_FOREACH (const IPoint &p, data_[i]->points_) {
      for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
          for (int dz = -1; dz <= 1; ++dz) {
            const IPoint q(p.x_ + dx, p.y_ + dy, p.z_ + dz);
            auto range = coord_to_index.equal_range(q);
            for (auto it = range.first; it != range.second; ++it) {
              if (it->second != i) {
                data_[i]->AddConnection(data_[it->second].get());
              }
            }
          }
        }
      }
    }
  }
}

void Builder::CutLoops() {
  assert(is_radius_computed_);
  // use kruskal like algorithm
  // see https://en.wikipedia.org/wiki/Kruskal%27s_algorithm
  DisjointSet<Cluster *> U;
  std::vector<std::pair<double, std::pair<Cluster *, Cluster *> > > E;
  BOOST_FOREACH (ClusterPtr cls, data_) {
    U.Add(cls.get());
    BOOST_FOREACH (Cluster *adj, cls->adjacent_) {
      double strength = (cls->radius_ + adj->radius_) / 2.0;
      Cluster *a = cls.get(), *b = adj;
      if (a < b)
        E.push_back(std::make_pair(strength, std::make_pair(cls.get(), adj)));
    }
  }
  U.SetUp();
  std::sort(E.begin(), E.end());
  std::reverse(E.begin(), E.end());
  for (const std::pair<double, std::pair<Cluster *, Cluster *> > &it : E) {
    Cluster *a = it.second.first;
    Cluster *b = it.second.second;
    if (U.IsSame(a, b)) {
      a->RemoveConnection(b);
      b->RemoveConnection(a);
    } else {
      U.Merge(a, b);
    }
  }
}

static NeuronNode *findEdgeNode(NeuronNode *node) {
  std::queue<NeuronNode *> que;
  NeuronNode *last = node;
  for (int i = 0; i < 2; ++i) {
    std::set<NeuronNode *> used;
    que.push(last);
    used.insert(last);
    while (!que.empty()) {
      NeuronNode *cur = que.front();
      que.pop();
      last = cur;
      BOOST_FOREACH (NeuronNode *next, cur->adjacent_) {
        if (used.count(next))
          continue;
        que.push(next);
        used.insert(next);
      }
    }
  }
  return last;
}

void Builder::ComputeGravityPoints() {
  BOOST_FOREACH (ClusterPtr cls, data_) {
    assert(!cls->points_.empty());
    double sx = 0, sy = 0, sz = 0;
    BOOST_FOREACH (const IPoint &p, cls->points_) {
      sx += p.x_;
      sy += p.y_;
      sz += p.z_;
    }
    cls->gx_ = sx / cls->points_.size();
    cls->gy_ = sy / cls->points_.size();
    cls->gz_ = sz / cls->points_.size();
  }
}

void Builder::ComputeRadius() {
  BOOST_FOREACH (ClusterPtr cls, data_) {
    double mdx = 0, mdy = 0, mdz = 0;
    BOOST_FOREACH (const IPoint &p, cls->points_) {
      mdx = std::max(mdx, scale_xy_ * std::abs(p.x_ - cls->gx_));
      mdy = std::max(mdy, scale_xy_ * std::abs(p.y_ - cls->gy_));
      mdz = std::max(mdz, scale_z_ * std::abs(p.z_ - cls->gz_));
    }
    cls->radius_ = std::sqrt(mdx * mdx + mdy * mdy + mdz * mdz);
  }
  is_radius_computed_ = true;
}

std::vector<NeuronNodePtr>
Builder::ConvertToNeuronNodes(std::vector<ClusterPtr> &data,
                                const double scale_xy, const double scale_z) {
  std::vector<NeuronNodePtr> neuron_nodes;
  std::vector<std::pair<int, int> > edges;
  for (int i = 0; i < (int)data.size(); ++i) {
    NeuronNodePtr n = boost::make_shared<NeuronNode>();
    n->setCoord(data[i]->gx_ * scale_xy,
             data[i]->gy_ * scale_xy,
             data[i]->gz_ * scale_z);
    n->radius_ = data[i]->radius_;
    neuron_nodes.push_back(n);
  }
  std::map<Cluster *, int> ptr2index;
  for (int i = 0; i < (int)data.size(); ++i) {
    ptr2index[data[i].get()] = i;
  }
  for (int i = 0; i < (int)data.size(); ++i) {
    for (Cluster *p : data[i]->adjacent_) {
      int j = ptr2index[p];
      if (i < j) {
        neuron_nodes[i]->AddConnection(neuron_nodes[j].get());
        neuron_nodes[j]->AddConnection(neuron_nodes[i].get());
      }
    }
  }
  return neuron_nodes;
}

std::vector<Neuron>
Builder::ConvertToNeuron(std::vector<ClusterPtr> &data,
                           const double scale_xy, const double scale_z) {
  std::vector<NeuronNodePtr> neuron_nodes =
      ConvertToNeuronNodes(data, scale_xy, scale_z);
  // split into some neurons
  std::set<NeuronNode *> used;
  std::vector<Neuron> neurons;
  std::map<NeuronNode *, NeuronNodePtr> ptr2smartptr;
  for (NeuronNodePtr p : neuron_nodes)
    ptr2smartptr[p.get()] = p;

  BOOST_FOREACH (NeuronNodePtr node, neuron_nodes) {
    if (used.count(node.get()))
      continue;
    // FIXME
    neurons.push_back(Neuron());
    Neuron &n = neurons.back();
    n.root_ = findEdgeNode(node.get());
    n.AddNode(node);
    std::stack<NeuronNode *> stk;
    stk.push(node.get());
    used.insert(node.get());
    while (!stk.empty()) {
      NeuronNode *cur = stk.top();
      stk.pop();
      BOOST_FOREACH (NeuronNode *next, cur->adjacent_) {
        // If next != parent
        if (!used.count(next)) {
          n.AddNode(ptr2smartptr[next]);
          stk.push(next);
          used.insert(next);
        }
      }
    }
  }
  return neurons;
}

static void computeIdInner(NeuronNode *cur, NeuronNode *prev, int &id) {
  cur->id_ = id++;
  BOOST_FOREACH (NeuronNode *next, cur->adjacent_) {
    if (next != prev) {
      computeIdInner(next, cur, id);
    }
  }
}

void Builder::ComputeIds(std::vector<Neuron> &ns) {
  int id = 1;
  for (int i = 0; i < (int)ns.size(); ++i) {
    computeIdInner(ns[i].root_, NULL, id);
  }
}

static void compute_node_type_inner(NeuronNode *cur, NeuronNode *prev) {
  neuron_type type;
  if (cur->adjacent_.size() >= 3)
    type = neuron_type::BRANCH;
  else if (cur->adjacent_.size() == 2)
    type = neuron_type::CONNECT;
  else
    type = neuron_type::EDGE;
  cur->type_ = type;
  BOOST_FOREACH (NeuronNode *next, cur->adjacent_) {
    if (next != prev) {
      compute_node_type_inner(next, cur);
    }
  }
}

void Builder::ComputeNodeTypes(std::vector<Neuron> &neu) {
  for (int i = 0; i < (int)neu.size(); ++i) {
    compute_node_type_inner(neu[i].root_, NULL);
  }
}

std::vector<Neuron> Builder::Build() {
  bool print_progress = true;
  ComputeGravityPoints();
  if (print_progress)
    std::cerr << "compute_gravity_point" << std::endl;
  ComputeRadius();
  if (print_progress)
    std::cerr << "compute_radius" << std::endl;
  ConnectNeighbors();
  if (print_progress)
    std::cerr << "connect_neighbor" << std::endl;
  CutLoops();
  if (print_progress)
    std::cerr << "cut_loops" << std::endl;
  std::vector<Neuron> neu = ConvertToNeuron(data_, scale_xy_, scale_z_);
  if (print_progress)
    std::cerr << "convert_to_neuron" << std::endl;
  ComputeIds(neu);
  if (print_progress)
    std::cerr << "compute_id" << std::endl;
  ComputeNodeTypes(neu);
  if (print_progress)
    std::cerr << "compute_node_type" << std::endl;
  return neu;
}
} // namespace sigen
