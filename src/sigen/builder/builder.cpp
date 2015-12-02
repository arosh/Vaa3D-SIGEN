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
builder::builder(const std::vector<boost::shared_ptr<cluster> > &data,
                 const double scale_xy, const double scale_z)
    : is_radius_computed_(false), data_(data), scale_xy_(scale_xy), scale_z_(scale_z) {}
void builder::connect_neighbor() {
  for (int i = 0; i < (int)data_.size(); ++i) {
    for (int j = i + 1; j < (int)data_.size(); ++j) {
      if (data_[i]->check_neighbor(data_[j].get())) {
        data_[i]->add_connection(data_[j].get());
        data_[j]->add_connection(data_[i].get());
      }
    }
  }
}

void builder::cut_loops() {
  assert(is_radius_computed_);
  // use kruskal like algorithm
  disjoint_set<cluster *> U;
  std::vector<std::pair<double, std::pair<cluster *, cluster *> > > E;
  for (boost::shared_ptr<cluster> cls : data_) {
    U.add(cls.get());
    for (cluster *adj : cls->adjacent_) {
      double strength = (cls->radius_ + adj->radius_) / 2.0;
      cluster *a = cls.get(), *b = adj;
      if (a < b)
        E.push_back(std::make_pair(strength, std::make_pair(cls.get(), adj)));
    }
  }
  U.setup();
  std::sort(E.begin(), E.end());
  std::reverse(E.begin(), E.end());
  for (const std::pair<double, std::pair<cluster *, cluster *> > &it : E) {
    cluster *a = it.second.first;
    cluster *b = it.second.second;
    if (U.same(a, b)) {
      std::vector<cluster *>::iterator iter;
      iter = std::remove(a->adjacent_.begin(), a->adjacent_.end(), b);
      a->adjacent_.erase(iter, a->adjacent_.end());
      iter = std::remove(b->adjacent_.begin(), b->adjacent_.end(), a);
      b->adjacent_.erase(iter, b->adjacent_.end());
    } else {
      U.merge(a, b);
    }
  }
}

void builder::connect_interpolate(double dt) {
  // 重くなりそうなので，重心の距離で
  // pixel resolution???
}

static neuron_node *find_edge(neuron_node *node) {
  std::queue<neuron_node *> que;
  neuron_node *last = node;
  for (int i = 0; i < 2; ++i) {
    std::set<neuron_node *> used;
    que.push(last);
    used.insert(last);
    while (!que.empty()) {
      neuron_node *cur = que.front();
      que.pop();
      last = cur;
      for (neuron_node *next : cur->adjacent_) {
        if (used.count(next))
          continue;
        que.push(next);
        used.insert(next);
      }
    }
  }
  return last;
}

void builder::compute_gravity_point() {
  for (boost::shared_ptr<cluster> cls : data_) {
    assert(!cls->points_.empty());
    double sx = 0, sy = 0, sz = 0;
    for (const point<int> &p : cls->points_) {
      sx += p.x_;
      sy += p.y_;
      sz += p.z_;
    }
    cls->gx_ = sx / cls->points_.size();
    cls->gy_ = sy / cls->points_.size();
    cls->gz_ = sz / cls->points_.size();
  }
}

void builder::compute_radius() {
  for (boost::shared_ptr<cluster> cls : data_) {
    double mdx = 0, mdy = 0, mdz = 0;
    for (const point<int> &p : cls->points_) {
      mdx = std::max(mdx, scale_xy_ * std::abs(p.x_ - cls->gx_));
      mdy = std::max(mdy, scale_xy_ * std::abs(p.y_ - cls->gy_));
      mdz = std::max(mdz, scale_z_ * std::abs(p.z_ - cls->gz_));
    }
    cls->radius_ = std::sqrt(mdx * mdx + mdy * mdy + mdz * mdz);
  }
  is_radius_computed_ = true;
}

static bool check_adjacent(const cluster *a, const cluster *b) {
  auto iter = std::find(a->adjacent_.begin(), a->adjacent_.end(), b);
  return iter != a->adjacent_.end();
}

std::vector<boost::shared_ptr<neuron_node> >
builder::convert_to_neuron_node(std::vector<boost::shared_ptr<cluster> > &data,
                                const double scale_xy, const double scale_z) {
  std::vector<boost::shared_ptr<neuron_node> > neuron_nodes;
  std::vector<std::pair<int, int> > edges;
  for (int i = 0; i < (int)data.size(); ++i) {
    auto n = boost::make_shared<neuron_node>();
    n->gx_ = data[i]->gx_ * scale_xy;
    n->gy_ = data[i]->gy_ * scale_xy;
    n->gz_ = data[i]->gz_ * scale_z;
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

std::vector<neuron>
builder::convert_to_neuron(std::vector<boost::shared_ptr<cluster> > &data,
                           const double scale_xy, const double scale_z) {
  std::vector<boost::shared_ptr<neuron_node> > neuron_nodes =
      convert_to_neuron_node(data, scale_xy, scale_z);
  // split into some neurons
  std::set<neuron_node *> used;
  std::vector<neuron> neurons;
  for (boost::shared_ptr<neuron_node> node : neuron_nodes) {
    if (used.count(node.get()))
      continue;
    neurons.push_back(neuron());
    neuron &n = neurons.back();
    n.root_ = find_edge(node.get());
    n.storage_.push_back(node);
    std::stack<neuron_node *> stk;
    stk.push(node.get());
    used.insert(node.get());
    while (!stk.empty()) {
      neuron_node *cur = stk.top();
      stk.pop();
      for (neuron_node *next : cur->adjacent_) {
        if (!used.count(next)) {
          for (boost::shared_ptr<neuron_node> inst : neuron_nodes) {
            if (inst.get() == next)
              n.storage_.push_back(inst);
          }
          stk.push(next);
          used.insert(next);
        }
      }
    }
  }
  return neurons;
}

static void compute_id_inner(neuron_node *cur, neuron_node *prev, int &id) {
  cur->id_ = id++;
  for (neuron_node *next : cur->adjacent_) {
    if (next != prev) {
      compute_id_inner(next, cur, id);
    }
  }
}

void builder::compute_id(std::vector<neuron> &ns) {
  int id = 1;
  for (int i = 0; i < (int)ns.size(); ++i) {
    compute_id_inner(ns[i].root_, NULL, id);
  }
}

static void compute_node_type_inner(neuron_node *cur, neuron_node *prev) {
  neuron_type type;
  if (cur->adjacent_.size() >= 3)
    type = neuron_type::BRANCH;
  else if (cur->adjacent_.size() == 2)
    type = neuron_type::CONNECT;
  else
    type = neuron_type::EDGE;
  cur->type_ = type;
  for (neuron_node *next : cur->adjacent_) {
    if (next != prev) {
      compute_node_type_inner(next, cur);
    }
  }
}

void builder::compute_node_type(std::vector<neuron> &neu) {
  for (int i = 0; i < (int)neu.size(); ++i) {
    compute_node_type_inner(neu[i].root_, NULL);
  }
}

std::vector<neuron> builder::build() {
  compute_gravity_point();
  compute_radius();
  connect_neighbor();
  cut_loops();
  std::vector<neuron> neu = convert_to_neuron(data_, scale_xy_, scale_z_);
  compute_id(neu);
  compute_node_type(neu);
  return neu;
}
} // namespace sigen
