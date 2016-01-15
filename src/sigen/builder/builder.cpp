#include "sigen/builder/builder.h"
#include "sigen/common/disjoint_set.h"
#include <cassert>
#include <cmath>
#include <algorithm>
#include <utility>
#include <vector>
#include <queue>
#include <stack>
#include <set>
#include <boost/foreach.hpp>
namespace sigen {
Builder::Builder(const std::vector<boost::shared_ptr<Cluster> > &data,
                 const double scale_xy, const double scale_z)
    : is_radius_computed_(false), data_(data), scale_xy_(scale_xy), scale_z_(scale_z) {}
void Builder::connect_neighbor() {
  for (int i = 0; i < (int)data_.size(); ++i) {
    for (int j = i + 1; j < (int)data_.size(); ++j) {
      if (data_[i]->check_neighbor(data_[j].get())) {
        data_[i]->add_connection(data_[j].get());
        data_[j]->add_connection(data_[i].get());
      }
    }
  }
}

void Builder::cut_loops() {
  assert(is_radius_computed_);
  // use kruskal like algorithm
  // see https://en.wikipedia.org/wiki/Kruskal%27s_algorithm
  DisjointSet<Cluster *> U;
  std::vector<std::pair<double, std::pair<Cluster *, Cluster *> > > E;
  for (boost::shared_ptr<Cluster> cls : data_) {
    U.add(cls.get());
    for (Cluster *adj : cls->adjacent_) {
      double strength = (cls->radius_ + adj->radius_) / 2.0;
      Cluster *a = cls.get(), *b = adj;
      if (a < b)
        E.push_back(std::make_pair(strength, std::make_pair(cls.get(), adj)));
    }
  }
  U.setup();
  std::sort(E.begin(), E.end());
  std::reverse(E.begin(), E.end());
  for (const std::pair<double, std::pair<Cluster *, Cluster *> > &it : E) {
    Cluster *a = it.second.first;
    Cluster *b = it.second.second;
    if (U.same(a, b)) {
      std::vector<Cluster *>::iterator iter;
      iter = std::remove(a->adjacent_.begin(), a->adjacent_.end(), b);
      a->adjacent_.erase(iter, a->adjacent_.end());
      iter = std::remove(b->adjacent_.begin(), b->adjacent_.end(), a);
      b->adjacent_.erase(iter, b->adjacent_.end());
    } else {
      U.merge(a, b);
    }
  }
}

static NeuronNode *find_edge(NeuronNode *node) {
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
      for (NeuronNode *next : cur->adjacent_) {
        if (used.count(next))
          continue;
        que.push(next);
        used.insert(next);
      }
    }
  }
  return last;
}

void Builder::compute_gravity_point() {
  for (boost::shared_ptr<Cluster> cls : data_) {
    assert(!cls->points_.empty());
    double sx = 0, sy = 0, sz = 0;
    for (const Point<int> &p : cls->points_) {
      sx += p.x_;
      sy += p.y_;
      sz += p.z_;
    }
    cls->gx_ = sx / cls->points_.size();
    cls->gy_ = sy / cls->points_.size();
    cls->gz_ = sz / cls->points_.size();
  }
}

void Builder::compute_radius() {
  for (boost::shared_ptr<Cluster> cls : data_) {
    double mdx = 0, mdy = 0, mdz = 0;
    for (const Point<int> &p : cls->points_) {
      mdx = std::max(mdx, scale_xy_ * std::abs(p.x_ - cls->gx_));
      mdy = std::max(mdy, scale_xy_ * std::abs(p.y_ - cls->gy_));
      mdz = std::max(mdz, scale_z_ * std::abs(p.z_ - cls->gz_));
    }
    cls->radius_ = std::sqrt(mdx * mdx + mdy * mdy + mdz * mdz);
  }
  is_radius_computed_ = true;
}

static bool check_adjacent(const Cluster *a, const Cluster *b) {
  auto iter = std::find(a->adjacent_.begin(), a->adjacent_.end(), b);
  return iter != a->adjacent_.end();
}

std::vector<boost::shared_ptr<NeuronNode> >
Builder::convert_to_neuron_node(std::vector<boost::shared_ptr<Cluster> > &data,
                                const double scale_xy, const double scale_z) {
  std::vector<boost::shared_ptr<NeuronNode> > neuron_nodes;
  std::vector<std::pair<int, int> > edges;
  for (int i = 0; i < (int)data.size(); ++i) {
    auto n = boost::make_shared<NeuronNode>();
    n->coord(data[i]->gx_ * scale_xy,
             data[i]->gy_ * scale_xy,
             data[i]->gz_ * scale_z);
    n->radius_ = data[i]->radius_;
    neuron_nodes.push_back(n);
    for (int j = i + 1; j < (int)data.size(); ++j) {
      if (check_adjacent(data[i].get(), data[j].get())) {
        edges.push_back(std::make_pair(i, j));
      }
    }
  }
  for (std::pair<int, int> item : edges) {
    int a = item.first, b = item.second;
    neuron_nodes[a]->add_connection(neuron_nodes[b].get());
    neuron_nodes[b]->add_connection(neuron_nodes[a].get());
  }
  return neuron_nodes;
}

std::vector<Neuron>
Builder::convert_to_neuron(std::vector<boost::shared_ptr<Cluster> > &data,
                           const double scale_xy, const double scale_z) {
  std::vector<boost::shared_ptr<NeuronNode> > neuron_nodes =
      convert_to_neuron_node(data, scale_xy, scale_z);
  // split into some neurons
  std::set<NeuronNode *> used;
  std::vector<Neuron> neurons;
  for (boost::shared_ptr<NeuronNode> node : neuron_nodes) {
    if (used.count(node.get()))
      continue;
    neurons.push_back(Neuron());
    Neuron &n = neurons.back();
    n.root_ = find_edge(node.get());
    n.add_node(node);
    std::stack<NeuronNode *> stk;
    stk.push(node.get());
    used.insert(node.get());
    while (!stk.empty()) {
      NeuronNode *cur = stk.top();
      stk.pop();
      for (NeuronNode *next : cur->adjacent_) {
        if (!used.count(next)) {
          for (boost::shared_ptr<NeuronNode> inst : neuron_nodes) {
            if (inst.get() == next)
              n.add_node(inst);
          }
          stk.push(next);
          used.insert(next);
        }
      }
    }
  }
  return neurons;
}

static void compute_id_inner(NeuronNode *cur, NeuronNode *prev, int &id) {
  cur->id_ = id++;
  for (NeuronNode *next : cur->adjacent_) {
    if (next != prev) {
      compute_id_inner(next, cur, id);
    }
  }
}

void Builder::compute_id(std::vector<Neuron> &ns) {
  int id = 1;
  for (int i = 0; i < (int)ns.size(); ++i) {
    compute_id_inner(ns[i].root_, NULL, id);
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
  for (NeuronNode *next : cur->adjacent_) {
    if (next != prev) {
      compute_node_type_inner(next, cur);
    }
  }
}

void Builder::compute_node_type(std::vector<Neuron> &neu) {
  for (int i = 0; i < (int)neu.size(); ++i) {
    compute_node_type_inner(neu[i].root_, NULL);
  }
}

std::vector<Neuron> Builder::build() {
  compute_gravity_point();
  compute_radius();
  connect_neighbor();
  cut_loops();
  std::vector<Neuron> neu = convert_to_neuron(data_, scale_xy_, scale_z_);
  compute_id(neu);
  compute_node_type(neu);
  return neu;
}
} // namespace sigen
