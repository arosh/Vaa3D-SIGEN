#include <gtest/gtest.h>
#include "extractor/extractor.h"

using namespace sigen;

TEST(extractor, labeling) {
  binary_cube cube(3, 3, 1);
  cube[0][0][0] = 1; cube[1][0][0] = 1;
  cube[0][2][0] = 1; cube[1][2][0] = 1; cube[2][2][0] = 1;
  extractor ext(cube);
}
