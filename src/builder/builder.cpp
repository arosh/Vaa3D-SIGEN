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
      mdx = std::max(mdx, scale_xy_ * abs(v->x_ - cls->gx_));
      mdy = std::max(mdy, scale_xy_ * abs(v->y_ - cls->gy_));
      mdz = std::max(mdz, scale_z_ * abs(v->z_ - cls->gz_));
    }
    cls->radius_ = std::sqrt(mdx * mdx + mdy * mdy + mdz * mdz);
  }
}
neuron builder::build() {
  neuron ret;
  return ret;
}
} // namespace sigen
