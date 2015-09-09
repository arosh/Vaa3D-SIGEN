#include <gtest/gtest.h>
#include <builder/builder.h>
#include <extractor/extractor.h>
using namespace sigen;
TEST(builder, connect_neighbor) {
  binary_cube cube(5, 5, 3);
  cube[1][1][1] = 1; cube[2][1][1] = 1;

  cube[1][3][1] = 1; cube[2][3][1] = 1; cube[3][3][1] = 1;
  extractor ext(cube);
  std::vector<std::shared_ptr<cluster>> data = ext.extract();
  builder bld(data, 1.0, 1.0);
  bld.connect_neighbor();
  EXPECT_EQ(5, (int)bld.data_.size());
  EXPECT_EQ(1, (int)bld.data_[0]->adjacent_.size());
  EXPECT_EQ(2, (int)bld.data_[1]->adjacent_.size());
  EXPECT_EQ(1, (int)bld.data_[2]->adjacent_.size());
  EXPECT_EQ(1, (int)bld.data_[3]->adjacent_.size());
  EXPECT_EQ(1, (int)bld.data_[4]->adjacent_.size());
}
TEST(builder, compute_gravity_point) {
  binary_cube cube(5, 5, 3);
  cube[1][1][1] = 1; cube[2][1][1] = 1;

  cube[1][3][1] = 1; cube[2][3][1] = 1; cube[3][3][1] = 1;
  extractor ext(cube);
  std::vector<std::shared_ptr<cluster>> data = ext.extract();
  builder bld(data, 1.0, 1.0);
  bld.connect_neighbor();
  bld.compute_gravity_point();
  EXPECT_DOUBLE_EQ(1.0, bld.data_[0]->gx_);
  EXPECT_DOUBLE_EQ(3.0, bld.data_[0]->gy_);
  EXPECT_DOUBLE_EQ(1.0, bld.data_[0]->gz_);
  EXPECT_DOUBLE_EQ(2.0, bld.data_[4]->gx_);
  EXPECT_DOUBLE_EQ(1.0, bld.data_[4]->gy_);
  EXPECT_DOUBLE_EQ(1.0, bld.data_[4]->gz_);
}
TEST(builder, convert_to_neuron_node) {
  binary_cube cube(5, 5, 3);
  cube[1][1][1] = 1; cube[2][1][1] = 1;

  cube[1][3][1] = 1; cube[2][3][1] = 1; cube[3][3][1] = 1;
  extractor ext(cube);
  std::vector<std::shared_ptr<cluster>> data = ext.extract();
  builder bld(data, 1.0, 1.0);
  bld.connect_neighbor();
  bld.compute_gravity_point();
  bld.compute_radius();
  auto ret = bld.convert_to_neuron_node(bld.data_, bld.scale_xy_, bld.scale_z_);
}
