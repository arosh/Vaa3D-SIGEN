#include <gtest/gtest.h>
#include <extractor/extractor.h>
using namespace sigen;
TEST(extractor, labeling) {
  binary_cube cube(5, 5, 3);
  cube[1][1][1] = 1; cube[2][1][1] = 1;
  cube[1][3][1] = 1; cube[2][3][1] = 1; cube[3][3][1] = 1;
  extractor ext(cube);
  ext.extract();
  EXPECT_EQ(2, (int)ext.clusters_.size());
  EXPECT_EQ(3, (int)ext.clusters_[0].size());
  EXPECT_EQ(2, (int)ext.clusters_[1].size());
}
TEST(extractor, labeling2) {
  binary_cube cube(5, 5, 3);
  cube[1][1][1] = 1; cube[2][1][1] = 1;
                                        cube[3][2][1] = 1;
  cube[1][3][1] = 1; cube[2][3][1] = 1;
  extractor ext(cube);
  ext.extract();
  EXPECT_EQ(1, (int)ext.clusters_.size());
  EXPECT_EQ(5, (int)ext.clusters_[0].size());
}
