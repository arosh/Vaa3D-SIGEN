#include <iostream>
#include <gtest/gtest.h>
#include <extractor/extractor.h>
using namespace std;
using namespace sigen;
TEST(extractor, labeling) {
  binary_cube cube(5, 5, 3);
  cube[1][1][1] = 1; cube[2][1][1] = 1;

  cube[1][3][1] = 1; cube[2][3][1] = 1; cube[3][3][1] = 1;
  extractor ext(cube);
  auto ret = ext.extract();
  EXPECT_EQ(2, (int)ext.components_.size());
  EXPECT_EQ(3, (int)ext.components_[0].size());
  EXPECT_EQ(2, (int)ext.components_[1].size());
  EXPECT_EQ(5, (int)ret.size());
  for(auto c : ret) {
    EXPECT_EQ(1, (int)c->points_.size());
    // point(1,3,1)
    // point(2,3,1)
    // point(3,3,1)
    // point(1,1,1)
    // point(2,1,1)
    // cout << "point(" << c->points_[0].x_ << ',' << c->points_[0].y_ << "," << c->points_[0].z_ << ")" << endl;
  }
}
TEST(extractor, labeling2) {
  binary_cube cube(5, 5, 3);
  cube[1][1][1] = 1; cube[2][1][1] = 1;
                                        cube[3][2][1] = 1;
  cube[1][3][1] = 1; cube[2][3][1] = 1;
  extractor ext(cube);
  auto ret = ext.extract();
  EXPECT_EQ(1, (int)ext.components_.size());
  EXPECT_EQ(5, (int)ext.components_[0].size());
  EXPECT_EQ(5, (int)ret.size());
}
TEST(extractor, labeling_with_loops) {
  binary_cube cube(5, 5, 3);
                     cube[2][1][1] = 1;
  cube[1][2][1] = 1;                    cube[3][2][1] = 1;
                     cube[2][3][1] = 1;
  extractor ext(cube);
  auto ret = ext.extract();
  EXPECT_EQ(1, (int)ext.components_.size());
  EXPECT_EQ(4, (int)ext.components_[0].size());
  for(auto c : ret) {
    // point(1,2,1)
    // point(2,1,1)
    // point(2,3,1)
    // point(3,2,1)
    for(auto p : c->points_) {
    //   cout << "point(" << p.x_ << ',' << p.y_ << "," << p.z_ << ")" << endl;
    }
  }
}
TEST(extractor, same_distance) {
  binary_cube cube(4, 4, 3);
  cube[1][1][1] = 1; cube[2][1][1] = 1;
  cube[1][2][1] = 1; cube[2][2][1] = 1;
  extractor ext(cube);
  auto ret = ext.extract();
  EXPECT_EQ(1, (int)ext.components_.size());
  EXPECT_EQ(4, (int)ext.components_[0].size());
  EXPECT_EQ(2, (int)ret.size());
  EXPECT_EQ(3, (int)ret[0]->points_.size());
  EXPECT_EQ(1, (int)ret[1]->points_.size());
  for(auto c : ret) {
    // point(1,1,1)
    // point(1,2,1)
    // point(2,2,1)
    // point(2,1,1)
    for(auto p : c->points_) {
    //   cout << "point(" << p.x_ << ',' << p.y_ << "," << p.z_ << ")" << endl;
    }
  }
}
