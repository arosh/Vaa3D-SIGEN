#include "extractor.h"
#include <map>

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
void extractor::labeling() {
  clusters_.clear();
  // map<point, point> っぽいのになりそう。設計失敗した
}
}
