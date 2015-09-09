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
  bld.cut_loops();
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
  bld.cut_loops();
  bld.compute_gravity_point();
  bld.compute_radius();
  std::vector<std::shared_ptr<neuron_node>> nn = bld.convert_to_neuron_node(bld.data_, bld.scale_xy_, bld.scale_z_);
  EXPECT_EQ(5, (int)nn.size());
  EXPECT_EQ(1.0, nn[0]->gx_);
  EXPECT_EQ(3.0, nn[0]->gy_);
  EXPECT_EQ(1.0, nn[0]->gz_);
  EXPECT_EQ(2.0, nn[4]->gx_);
  EXPECT_EQ(1.0, nn[4]->gy_);
  EXPECT_EQ(1.0, nn[4]->gz_);
  EXPECT_EQ(1, (int)nn[0]->adjacent_.size());
  EXPECT_EQ(2, (int)nn[1]->adjacent_.size());
  EXPECT_EQ(1, (int)nn[2]->adjacent_.size());
  EXPECT_EQ(1, (int)nn[3]->adjacent_.size());
  EXPECT_EQ(1, (int)nn[4]->adjacent_.size());
  EXPECT_EQ(nn[1].get(), nn[0]->adjacent_[0]);
  EXPECT_EQ(nn[0].get(), nn[1]->adjacent_[0]);
  EXPECT_EQ(nn[2].get(), nn[1]->adjacent_[1]);
  EXPECT_EQ(nn[1].get(), nn[2]->adjacent_[0]);
  EXPECT_EQ(nn[4].get(), nn[3]->adjacent_[0]);
  EXPECT_EQ(nn[3].get(), nn[4]->adjacent_[0]);
}
TEST(builder, convert_to_neuron) {
  binary_cube cube(5, 5, 3);
  cube[1][1][1] = 1; cube[2][1][1] = 1;

  cube[1][3][1] = 1; cube[2][3][1] = 1; cube[3][3][1] = 1;
  extractor ext(cube);
  std::vector<std::shared_ptr<cluster>> data = ext.extract();
  builder bld(data, 1.0, 1.0);
  bld.connect_neighbor();
  bld.cut_loops();
  bld.compute_gravity_point();
  bld.compute_radius();
  std::vector<neuron> ns = bld.convert_to_neuron(bld.data_, bld.scale_xy_, bld.scale_z_);
  EXPECT_EQ(2, (int)ns.size());
  EXPECT_EQ(3, (int)ns[0].storage_.size());
  EXPECT_EQ(2, (int)ns[1].storage_.size());
}
TEST(builder, compute_id) {
  binary_cube cube(5, 5, 3);
  cube[1][1][1] = 1; cube[2][1][1] = 1;

  cube[1][3][1] = 1; cube[2][3][1] = 1; cube[3][3][1] = 1;
  extractor ext(cube);
  std::vector<std::shared_ptr<cluster>> data = ext.extract();
  builder bld(data, 1.0, 1.0);
  bld.connect_neighbor();
  bld.cut_loops();
  bld.compute_gravity_point();
  bld.compute_radius();
  std::vector<neuron> ns = bld.convert_to_neuron(bld.data_, bld.scale_xy_, bld.scale_z_);
  bld.compute_id(ns);
  EXPECT_EQ(1, ns[0].storage_[0]->id_);
  EXPECT_EQ(2, ns[0].storage_[1]->id_);
  EXPECT_EQ(3, ns[0].storage_[2]->id_);
  EXPECT_EQ(4, ns[1].storage_[0]->id_);
  EXPECT_EQ(5, ns[1].storage_[1]->id_);
}
TEST(builder, convert_to_neuron_node_loops) {
  binary_cube cube(5, 5, 3);
                     cube[2][1][1] = 1;
  cube[1][2][1] = 1;                    cube[3][2][1] = 1;
                     cube[2][3][1] = 1;
  extractor ext(cube);
  std::vector<std::shared_ptr<cluster>> data = ext.extract();
  builder bld(data, 1.0, 1.0);
  bld.connect_neighbor();
  bld.cut_loops();
  bld.compute_gravity_point();
  bld.compute_radius();
  std::vector<std::shared_ptr<neuron_node>> nn = bld.convert_to_neuron_node(bld.data_, bld.scale_xy_, bld.scale_z_);
  EXPECT_EQ(4, (int)nn.size());
  EXPECT_EQ(1.0, nn[0]->gx_);
  EXPECT_EQ(2.0, nn[0]->gy_);
  EXPECT_EQ(1.0, nn[0]->gz_);
  EXPECT_EQ(3.0, nn[3]->gx_);
  EXPECT_EQ(2.0, nn[3]->gy_);
  EXPECT_EQ(1.0, nn[3]->gz_);
  EXPECT_EQ(1, (int)nn[0]->adjacent_.size());
  EXPECT_EQ(2, (int)nn[1]->adjacent_.size());
  EXPECT_EQ(1, (int)nn[2]->adjacent_.size());
  EXPECT_EQ(2, (int)nn[3]->adjacent_.size());
  EXPECT_EQ(nn[1].get(), nn[0]->adjacent_[0]);
  EXPECT_EQ(nn[0].get(), nn[1]->adjacent_[0]);
  EXPECT_EQ(nn[3].get(), nn[1]->adjacent_[1]);
  EXPECT_EQ(nn[3].get(), nn[2]->adjacent_[0]);
  EXPECT_EQ(nn[1].get(), nn[3]->adjacent_[0]);
  EXPECT_EQ(nn[2].get(), nn[3]->adjacent_[1]);
}
TEST(builder, convert_to_neuron_loops) {
  binary_cube cube(5, 5, 3);
                     cube[2][1][1] = 1;
  cube[1][2][1] = 1;                    cube[3][2][1] = 1;
                     cube[2][3][1] = 1;
  extractor ext(cube);
  std::vector<std::shared_ptr<cluster>> data = ext.extract();
  builder bld(data, 1.0, 1.0);
  bld.connect_neighbor();
  bld.cut_loops();
  bld.compute_gravity_point();
  bld.compute_radius();
  std::vector<neuron> ns = bld.convert_to_neuron(bld.data_, bld.scale_xy_, bld.scale_z_);
  EXPECT_EQ(1, (int)ns.size());
  EXPECT_EQ(4, (int)ns[0].storage_.size());
}
TEST(builder, compute_id_with_loops) {
  binary_cube cube(5, 5, 3);
                     cube[2][1][1] = 1;
  cube[1][2][1] = 1;                    cube[3][2][1] = 1;
                     cube[2][3][1] = 1;
  extractor ext(cube);
  std::vector<std::shared_ptr<cluster>> data = ext.extract();
  builder bld(data, 1.0, 1.0);
  bld.connect_neighbor();
  bld.cut_loops();
  bld.compute_gravity_point();
  bld.compute_radius();
  std::vector<neuron> ns = bld.convert_to_neuron(bld.data_, bld.scale_xy_, bld.scale_z_);
  bld.compute_id(ns);
}
