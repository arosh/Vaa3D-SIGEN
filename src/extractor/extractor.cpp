#include "extractor.h"

#define REP(i, n) for (int i = 0; i < (int)(n); ++i)

namespace sigen {
extractor::extractor(const binary_cube &cube) : c(cube) {}
void clear_frame() {
  REP(i, c.x) REP(j, c.y) {
    c[i][j][0] = false;
    c[i][j][c.z - 1] = false;
  }
  REP(j, c.y) REP(k, c.z) {
    c[0][j][k] = false;
    c[c.x - 1][j][k] = false;
  }
  REP(k, c.z) REP(i, c.x) {
    c[i][0][k] = false;
    c[i][c.y - 1][k] = false;
  }
}
void extracto::remove_isolation_point() {
  clear_frame(c);
}
}
