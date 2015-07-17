#include "extractor.h"
#include <map>
#include <tuple>
#include <algorithm>
#include <iterator>
#include <queue>
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
void before_filter(binary_cube &c) {
  clear_frame(c);
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
void set_label(Point p, const int label) {
  p->flag_ = true;
  p->label_ = label;
  for (auto next__ : p->adjacent_) {
    auto next = next__.lock();
    CHECK(next);
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
  std::map<std::tuple<int, int, int>, Point> points;
  for (int x = 1; x < cube_.x_ - 1; ++x) {
    for (int y = 1; y < cube_.y_ - 1; ++y) {
      for (int z = 1; z < cube_.z_ - 1; ++z) {
        if (cube_[x][y][z]) {
          points[std::tie(x, y, z)] = point_owner_.add(std::make_shared<point>(x, y, z));
        }
      }
    }
  }
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
  for (auto p : points)
    p.second->flag_ = false;
  int label = 0;
  for (auto p : points) {
    if (p.second->flag_ == false) {
      set_label(p.second, label++);
    }
  }
  clusters_.assign(label, std::vector<Point>());
  for (auto p : points) {
    clusters_[p.second->label_].push_back(p.second);
  }
  LOG(INFO) << "labeling end";
  typedef decltype(clusters_)::value_type V;
  LOG(INFO) << "sort by volume start";
  std::sort(std::begin(clusters_), end(clusters_),
            [](V lhs, V rhs) -> bool { return lhs.size() > rhs.size(); });
  LOG(INFO) << "sort by volume end";
}
Point find_single_seed(std::vector<Point> cluster) {
  CHECK(!cluster.empty());
  for (auto p : cluster)
    p->flag_ = false;
  std::queue<Point> que;
  cluster[0]->flag_ = true;
  que.push(cluster[0]);
  Point last;
  while (!que.empty()) {
    last = que.front();
    que.pop();
    for (auto next__ : last->adjacent_) {
      auto next = next__.lock();
      CHECK(next);
      if (!next->flag_) {
        next->flag_ = true;
        que.push(next);
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
int set_distance(std::vector<Point> cluster, Point seed) {
  for (auto p : cluster)
    p->flag_ = false;
  std::queue<Point> que;
  seed->flag_ = true;
  seed->label_ = 0;
  que.push(seed);
  int ret = 0;
  while (!que.empty()) {
    Point p = que.front();
    que.pop();
    ret = p->label_;
    for (auto next__ : p->adjacent_) {
      auto next = next__.lock();
      CHECK(next);
      if (!next->flag_) {
        next->flag_ = true;
        next->label_ = p->label_ + 1;
        que.push(next);
      }
    }
  }
  return ret;
}
std::vector<Point> extract_same_distance(Point seed) {
  std::vector<Point> ret;
  std::queue<Point> que;
  seed->flag_ = true;
  ret.push_back(seed);
  que.push(seed);
  while (!que.empty()) {
    Point p = que.front();
    que.pop();
    for (auto next__ : p->adjacent_) {
      auto next = next__.lock();
      if (!next->flag_ && next->label_ == seed->label_) {
        next->flag_ = true;
        ret.push_back(next);
        que.push(next);
      }
    }
  }
  return ret;
}
void extractor::extract() {
  labeling();
  LOG(INFO) << "extract start";
  for (auto &&cluster : clusters_) {
    auto seed = find_single_seed(cluster);
    int max_distance = set_distance(cluster, seed);
    // len(0 .. max_distance) = max_distance + 1
    std::vector<std::vector<Point>> dcluster(max_distance + 1);
    for (auto p : cluster)
      p->flag_ = false;
    for (auto p : cluster) {
      if (p->flag_ == false) {
        auto ret = extract_same_distance(p);
        // add range
        dcluster[p->label_].insert(dcluster[p->label_].end(), ret.begin(), ret.end());
      }
    }
  }
  LOG(INFO) << "extract end";
}
}
