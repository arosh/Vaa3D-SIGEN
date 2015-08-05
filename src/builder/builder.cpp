#include "./builder.h"
#include <cmath>
#include <algorithm>
#include <utility>
#include <vector>
#include <queue>
#include <set>
#include <glog/logging.h>
namespace sigen {
builder::builder(const std::vector<std::shared_ptr<cluster>> &data, const double scale_xy, const double scale_z)
    : data_(data), scale_xy_(scale_xy), scale_z_(scale_z) {}
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
  for (std::shared_ptr<cluster> cls : data_) {
    CHECK(!cls->voxels_.empty());
    double sx = 0, sy = 0, sz = 0;
    for (voxel *v : cls->voxels_) {
      sx += v->x_;
      sy += v->y_;
      sz += v->z_;
    }
    cls->gx_ = sx / cls->voxels_.size();
    cls->gy_ = sy / cls->voxels_.size();
    cls->gz_ = sz / cls->voxels_.size();
  }
}
void builder::compute_radius() {
  for (std::shared_ptr<cluster> cls : data_) {
    double mdx = 0, mdy = 0, mdz = 0;
    for (voxel *v : cls->voxels_) {
      mdx = std::max(mdx, scale_xy_ * std::abs(v->x_ - cls->gx_));
      mdy = std::max(mdy, scale_xy_ * std::abs(v->y_ - cls->gy_));
      mdz = std::max(mdz, scale_z_ * std::abs(v->z_ - cls->gz_));
    }
    cls->radius_ = std::sqrt(mdx * mdx + mdy * mdy + mdz * mdz);
  }
}
std::vector<neuron> builder::convert_to_neuron(std::vector<std::shared_ptr<cluster>> &data, const double scale_xy, const double scale_z) {
  std::vector<std::shared_ptr<neuron_node>> neuron_nodes;
  std::vector<std::pair<int, int>> conn;
  for (int i = 0; i < (int)data.size(); ++i) {
    auto n = std::make_shared<neuron_node>();
    n->gx_ = data[i]->gx_ * scale_xy;
    n->gy_ = data[i]->gy_ * scale_xy;
    n->gz_ = data[i]->gz_ * scale_z;
    n->radius_ = data[i]->radius_;
    neuron_nodes.push_back(n);
    for (int j = i + 1; j < (int)data.size(); ++j) {
      if (std::find(data[i]->adjacent_.begin(), data[i]->adjacent_.end(), data[j].get()) != data[i]->adjacent_.end()) {
        conn.emplace_back(i, j);
      }
    }
  }
  for (std::pair<int, int> item : conn) {
    int a = item.first, b = item.second;
    neuron_nodes[a]->add_connection(neuron_nodes[b].get());
    neuron_nodes[b]->add_connection(neuron_nodes[a].get());
  }
  std::set<neuron_node *> used;
  std::vector<neuron> neurons;
  for (std::shared_ptr<neuron_node> node : neuron_nodes) {
    if (used.count(node.get()))
      continue;
    neuron n;
    n.root_ = find_edge(node.get());
    std::queue<neuron_node *> que;
    n.data_.push_back(node);
    used.insert(node.get());
    que.push(node.get());
    while (!que.empty()) {
      neuron_node *cur = que.front();
      que.pop();
      for (neuron_node *next : cur->adjacent_) {
        if (used.count(next))
          continue;
        for (std::shared_ptr<neuron_node> ptr : neuron_nodes) {
          if (ptr.get() == next)
            n.data_.push_back(ptr);
        }
        used.insert(next);
        que.push(next);
      }
    }
  }
  return neurons;
}
static void compute_id_inner(neuron_node *cur, neuron_node *prev, int &id) {
  cur->id_ = id++;
  for (neuron_node *next : cur->adjacent_) {
    if (next == prev)
      continue;
    compute_id_inner(next, cur, id);
  }
}
void builder::compute_id(std::vector<neuron> &neu) {
  int id = 1;
  for (int i = 0; i < (int)neu.size(); ++i) {
    compute_id_inner(neu[i].root_, nullptr, id);
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
    if (next == prev)
      continue;
    compute_node_type_inner(next, cur);
  }
}
void builder::compute_node_type(std::vector<neuron> &neu) {
  for (int i = 0; i < (int)neu.size(); ++i) {
    compute_node_type_inner(neu[i].root_, nullptr);
  }
}
std::vector<neuron> builder::build() {
  connect_neighbor();
  compute_gravity_point();
  compute_radius();
  std::vector<neuron> neu = convert_to_neuron(data_, scale_xy_, scale_z_);
  compute_id(neu);
  compute_node_type(neu);
  return neu;
}
} // namespace sigen
