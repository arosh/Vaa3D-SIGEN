#include "./extractor.h"
#include <map>
#include <tuple>
#include <algorithm>
#include <iterator>
#include <queue>
#include <vector>
#include <glog/logging.h>

#define REP(i, n) for (int i = 0; i < (int)(n); ++i)

namespace sigen {
extractor::extractor(const binary_cube &cube) : cube_(cube) {}
void clear_frame(binary_cube &c) {
  REP(i, c.x_) REP(j, c.y_) {
    c[i][j][0] = false;
    c[i][j][c.z_ - 1] = false;
  }
  REP(j, c.y_) REP(k, c.z_) {
    c[0][j][k] = false;
    c[c.x_ - 1][j][k] = false;
  }
  REP(k, c.z_) REP(i, c.x_) {
    c[i][0][k] = false;
    c[i][c.y_ - 1][k] = false;
  }
}
void remove_isolation_point(binary_cube &c) {
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
void before_filter(binary_cube &c) {
  clear_frame(c);
  remove_isolation_point(c);
}
void set_label(point *p, const int label) {
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
  point_owner_.clear();
  std::map<std::tuple<int, int, int>, point *> points;
  for (int x = 1; x < cube_.x_ - 1; ++x) {
    for (int y = 1; y < cube_.y_ - 1; ++y) {
      for (int z = 1; z < cube_.z_ - 1; ++z) {
        if (cube_[x][y][z]) {
          point_owner_.push_back(std::make_shared<point>(x, y, z));
          points[std::tie(x, y, z)] = point_owner_.back().get();
        }
      }
    }
  }
  LOG(INFO) << "connecting begin";
  for (auto p : points) {
    // enumerate 26 neighbors
    for (int dx = -1; dx <= 1; ++dx) {
      for (int dy = -1; dy <= 1; ++dy) {
        for (int dz = -1; dz <= 1; ++dz) {
          if (dx == 0 && dy == 0 && dz == 0)
            continue;
          int x = std::get<0>(p.first) + dx;
          int y = std::get<1>(p.first) + dy;
          int z = std::get<2>(p.first) + dz;
          auto t = std::tuple<int, int, int>(x, y, z);
          if (points.count(t)) {
            p.second->add_connection(points[t]);
          }
        }
      }
    }
  }
  LOG(INFO) << "connecting end";
  for (auto p : points)
    p.second->flag_ = false;
  int label = 0;
  for (auto p : points) {
    if (p.second->flag_ == false) {
      set_label(p.second, label++);
    }
  }
  clusters_.assign(label, std::vector<point *>());
  for (auto p : points) {
    clusters_[p.second->label_].push_back(p.second);
  }
  typedef decltype(clusters_)::value_type V;
  std::sort(std::begin(clusters_), end(clusters_),
            [](V lhs, V rhs) -> bool { return lhs.size() > rhs.size(); });
  LOG(INFO) << "labeling end";
}
point *find_single_seed(std::vector<point *> cluster) {
  CHECK(!cluster.empty());
  point *last = cluster[0];
  for (int i = 0; i < 2; ++i) {
    for (auto p : cluster)
      p->flag_ = false;
    last->flag_ = true;
    std::queue<point *> que;
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
/*
 * Returns
 * =======
 * ret: max_distance of Point from seed
 */
void set_distance(std::vector<point *> cluster, point *seed) {
  for (auto p : cluster)
    p->flag_ = false;
  std::queue<point *> que;
  seed->flag_ = true;
  seed->label_ = 0;
  que.push(seed);
  while (!que.empty()) {
    point *p = que.front();
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
std::vector<point *> extract_same_distance(point *seed) {
  std::vector<point *> ret;
  std::queue<point *> que;
  seed->flag_ = true;
  ret.push_back(seed);
  que.push(seed);
  while (!que.empty()) {
    point *p = que.front();
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
std::vector<std::shared_ptr<point_link>> extractor::extract() {
  labeling();
  LOG(INFO) << "extract start";
  std::vector<std::shared_ptr<point_link>> ret;
  for (auto &&cluster : clusters_) {
    auto seed = find_single_seed(cluster);
    set_distance(cluster, seed);
    for (auto p : cluster)
      p->flag_ = false;
    for (auto p : cluster) {
      if (p->flag_ == false) {
        auto &&pp = extract_same_distance(p);
        ret.push_back(std::make_shared<point_link>(std::move(pp)));
      }
    }
  }
  LOG(INFO) << "extract end";
  return ret;
}
} // namespace sigen
