#include "./extractor.h"
#include "../common/point.h"
#include <map>
#include <algorithm>
#include <iterator>
#include <queue>
#include <vector>
#include <glog/logging.h>
#define REP(i, n) for (int i = 0; i < (int)(n); ++i)
namespace sigen {
extractor::extractor(const binary_cube &cube) : cube_(cube) {}
static void clear_frame(binary_cube &c) {
  REP(i, c.x_)
  REP(j, c.y_) {
    c[i][j][0] = false;
    c[i][j][c.z_ - 1] = false;
  }
  REP(j, c.y_)
  REP(k, c.z_) {
    c[0][j][k] = false;
    c[c.x_ - 1][j][k] = false;
  }
  REP(k, c.z_)
  REP(i, c.x_) {
    c[i][0][k] = false;
    c[i][c.y_ - 1][k] = false;
  }
}
static void remove_isolation_point(binary_cube &c) {
  binary_cube cc = c;
  for (int x = 1; x < c.x_ - 1; ++x) {
    for (int y = 1; y < c.y_ - 1; ++y) {
      for (int z = 1; z < c.z_ - 1; ++z) {
        bool any = false;
        for (int dx = -1; dx <= 1; ++dx) {
          for (int dy = -1; dy <= 1; ++dy) {
            for (int dz = -1; dz <= 1; ++dz) {
              if (dx == 0 && dy == 0 && dz == 0)
                continue;
              if (cc[x + dx][y + dy][z + dz])
                any = true;
            }
          }
        }
        if (!any)
          c[x][y][z] = false;
      }
    }
  }
}
static void before_filter(binary_cube &c) {
  clear_frame(c);
  remove_isolation_point(c);
}
static void set_label(voxel *p, const int label) {
  p->flag_ = true;
  p->label_ = label;
  for (auto next : p->adjacent_) {
    if (next->flag_ == false) {
      set_label(next, label);
    }
  }
}
void extractor::labeling() {
  LOG(INFO) << "before_filter start";
  before_filter(cube_);
  LOG(INFO) << "before_filter end";
  LOG(INFO) << "labeling start";
  voxel_owner_.clear();
  std::map<point<int>, voxel *> voxels;
  for (int x = 1; x < cube_.x_ - 1; ++x) {
    for (int y = 1; y < cube_.y_ - 1; ++y) {
      for (int z = 1; z < cube_.z_ - 1; ++z) {
        if (cube_[x][y][z]) {
          voxel_owner_.push_back(std::make_shared<voxel>(x, y, z));
          voxels[point<int>(x, y, z)] = voxel_owner_.back().get();
        }
      }
    }
  }
  LOG(INFO) << "connecting begin";
  for (auto p : voxels) {
    // enumerate 26 neighbors
    for (int dx = -1; dx <= 1; ++dx) {
      for (int dy = -1; dy <= 1; ++dy) {
        for (int dz = -1; dz <= 1; ++dz) {
          if (dx == 0 && dy == 0 && dz == 0)
            continue;
          int x = p.first.x_ + dx;
          int y = p.first.y_ + dy;
          int z = p.first.z_ + dz;
          point<int> t(x, y, z);
          if (voxels.count(t)) {
            p.second->add_connection(voxels[t]);
          }
        }
      }
    }
  }
  LOG(INFO) << "connecting end";
  for (auto p : voxels)
    p.second->flag_ = false;
  int label = 0;
  for (auto p : voxels) {
    if (p.second->flag_ == false) {
      set_label(p.second, label++);
    }
  }
  groups_.assign(label, std::vector<voxel *>());
  for (auto p : voxels) {
    groups_[p.second->label_].push_back(p.second);
  }
  typedef decltype(groups_)::value_type V;
  std::sort(std::begin(groups_), end(groups_),
            [](V lhs, V rhs) -> bool { return lhs.size() > rhs.size(); });
  LOG(INFO) << "labeling end";
}
static void reset_flag(std::vector<voxel *> &voxels) {
  for (auto p : voxels)
    p->flag_ = false;
}
static voxel *find_single_seed(std::vector<voxel *> &group) {
  CHECK(!group.empty());
  voxel *last = group[0];
  for (int i = 0; i < 2; ++i) {
    reset_flag(group);
    last->flag_ = true;
    std::queue<voxel *> que;
    que.push(last);
    while (!que.empty()) {
      last = que.front();
      que.pop();
      for (auto next : last->adjacent_) {
        if (!next->flag_) {
          next->flag_ = true;
          que.push(next);
        }
      }
    }
  }
  return last;
}
static void set_distance(std::vector<voxel *> &group, voxel *seed) {
  reset_flag(group);
  std::queue<voxel *> que;
  seed->flag_ = true;
  seed->label_ = 0;
  que.push(seed);
  while (!que.empty()) {
    voxel *p = que.front();
    que.pop();
    for (auto next : p->adjacent_) {
      if (!next->flag_) {
        next->flag_ = true;
        next->label_ = p->label_ + 1;
        que.push(next);
      }
    }
  }
}
static std::vector<voxel *> extract_same_distance(voxel *seed) {
  std::vector<voxel *> ret;
  std::queue<voxel *> que;
  seed->flag_ = true;
  ret.push_back(seed);
  que.push(seed);
  while (!que.empty()) {
    voxel *p = que.front();
    que.pop();
    for (auto next : p->adjacent_) {
      if (!next->flag_ && next->label_ == seed->label_) {
        next->flag_ = true;
        ret.push_back(next);
        que.push(next);
      }
    }
  }
  return ret;
}
static std::vector<point<int>> voxels_to_points(const std::vector<voxel *> vs) {
  std::vector<point<int>> ps;
  for (const voxel *v : vs) {
    ps.emplace_back(v->x_, v->y_, v->z_);
  }
  return ps;
}
std::vector<std::shared_ptr<cluster>> extractor::extract() {
  labeling();
  LOG(INFO) << "extract start";
  std::vector<std::shared_ptr<cluster>> ret;
  for (auto &&group : groups_) {
    auto seed = find_single_seed(group);
    set_distance(group, seed);
    reset_flag(group);
    for (auto p : group) {
      if (p->flag_ == false) {
        std::vector<voxel *> vs = extract_same_distance(p);
        std::vector<point<int>> ps = voxels_to_points(vs);
        ret.push_back(std::make_shared<cluster>(ps));
      }
    }
  }
  LOG(INFO) << "extract end";
  return ret;
}
} // namespace sigen
