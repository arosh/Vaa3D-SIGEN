#include "extractor.h"
#include <map>
#include <tuple>
#include <algorithm>
#include <iterator>

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
void set_label(std::shared_ptr<point> p, const int label) {
  p->flag_ = true;
  p->label_ = label;
  for(auto next : p->adjacent_) {
    if(next->flag_ == false) set_label(next, label);
  }
}
void extractor::labeling() {
  std::map<std::tuple<int, int, int>, std::shared_ptr<point>> points;
  for (int x = 1; x < cube_.x_ - 1; ++x) {
    for (int y = 1; y < cube_.y_ - 1; ++y) {
      for (int z = 1; z < cube_.z_ - 1; ++z) {
        if(cube_[x][y][z]) {
          points[std::tie(x, y, z)] = std::make_shared<point>(x, y, z);
        }
      }
    }
  }
  for(auto p : points) {
    for(int dx = -1; dx <= 1; ++dx) {
      for (int dy = -1; dy <= 1; ++dy) {
        for (int dz = -1; dz <= 1; ++dz) {
          if (dx == 0 && dy == 0 && dz == 0)
            continue;
          int x = std::get<0>(p.first) + dx;
          int y = std::get<1>(p.first) + dy;
          int z = std::get<2>(p.first) + dz;
          auto t = std::tuple<int, int, int>(x, y, z);
          if(points.count(t)) {
            p.second->add_connection(points[t]);
          }
        }
      }
    }
  }
  for(auto p : points) p.second->flag_ = false;
  int label = 0;
  for(auto p : points) {
    if(p.second->flag_ == false) {
      set_label(p.second, label++);
    }
  }
  clusters_.assign(label, std::vector<std::shared_ptr<point>>());
  for(auto p : points) {
    clusters_[p.second->label_].push_back(p.second);
  }
  typedef decltype(clusters_)::value_type V;
  std::sort(std::begin(clusters_), end(clusters_),
      [](V lhs, V rhs) -> bool { return lhs.size() > rhs.size(); });
}
}
