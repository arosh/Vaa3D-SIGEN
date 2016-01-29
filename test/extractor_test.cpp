#include <iostream>
#include <gtest/gtest.h>
#include "sigen/extractor/extractor.h"
using namespace std;
using namespace sigen;
TEST(Extractor, labeling) {
  BinaryCube cube(5, 5, 3);
  cube[1][1][1] = 1;
  cube[2][1][1] = 1;

  cube[1][3][1] = 1;
  cube[2][3][1] = 1;
  cube[3][3][1] = 1;
  Extractor ext(cube);
  auto ret = ext.extract();
  EXPECT_EQ(2, (int)ext.components_.size());
  EXPECT_EQ(3, (int)ext.components_[0].size());
  EXPECT_EQ(2, (int)ext.components_[1].size());
  EXPECT_EQ(5, (int)ret.size());
  for (auto c : ret) {
    EXPECT_EQ(1, (int)c->points_.size());
    // point(1,3,1)
    // point(2,3,1)
    // point(3,3,1)
    // point(1,1,1)
    // point(2,1,1)
    // cout << "point(" << c->points_[0].x_ << ',' << c->points_[0].y_ << "," << c->points_[0].z_ << ")" << endl;
  }
}
TEST(Extractor, labeling2) {
  BinaryCube cube(5, 5, 3);
  cube[1][1][1] = 1;
  cube[2][1][1] = 1;
  cube[3][2][1] = 1;
  cube[1][3][1] = 1;
  cube[2][3][1] = 1;
  Extractor ext(cube);
  auto ret = ext.extract();
  EXPECT_EQ(1, (int)ext.components_.size());
  EXPECT_EQ(5, (int)ext.components_[0].size());
  EXPECT_EQ(5, (int)ret.size());
}
TEST(Extractor, labeling_with_loops) {
  BinaryCube cube(5, 5, 3);
  cube[2][1][1] = 1;
  cube[1][2][1] = 1;
  cube[3][2][1] = 1;
  cube[2][3][1] = 1;
  Extractor ext(cube);
  auto ret = ext.extract();
  EXPECT_EQ(1, (int)ext.components_.size());
  EXPECT_EQ(4, (int)ext.components_[0].size());
  for (auto c : ret) {
    // point(1,2,1)
    // point(2,1,1)
    // point(2,3,1)
    // point(3,2,1)
    for (auto p : c->points_) {
      //   cout << "point(" << p.x_ << ',' << p.y_ << "," << p.z_ << ")" << endl;
    }
  }
}
TEST(Extractor, same_distance) {
  BinaryCube cube(4, 4, 3);
  cube[1][1][1] = 1;
  cube[2][1][1] = 1;
  cube[1][2][1] = 1;
  cube[2][2][1] = 1;
  Extractor ext(cube);
  auto ret = ext.extract();
  EXPECT_EQ(1, (int)ext.components_.size());
  EXPECT_EQ(4, (int)ext.components_[0].size());
  EXPECT_EQ(2, (int)ret.size());
  EXPECT_EQ(3, (int)ret[0]->points_.size());
  EXPECT_EQ(1, (int)ret[1]->points_.size());
  for (auto c : ret) {
    // point(1,1,1)
    // point(1,2,1)
    // point(2,2,1)
    // point(2,1,1)
    for (auto p : c->points_) {
      //   cout << "point(" << p.x_ << ',' << p.y_ << "," << p.z_ << ")" << endl;
    }
  }
}
