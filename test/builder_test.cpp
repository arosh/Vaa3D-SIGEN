#include <cassert>
#include <vector>
#include <string>
#include <gtest/gtest.h>
#include "sigen/builder/builder.h"
#include "sigen/extractor/extractor.h"
using namespace sigen;
static BinaryCube vectorStringToBinaryCube(const std::vector<std::string> &vs) {
  const int zdepth = 3;
  const int size_x = vs[0].size();
  const int size_y = vs.size();
  for(int i = 0; i < (int)size_y; ++i) {
    assert(size_x == (int)vs[i].size());
  }
  BinaryCube cube(size_x+2, size_y+2, zdepth);
  for(int y = 0; y < size_y; ++y) {
    for(int x = 0; x < size_x; ++x) {
      cube[x + 1][y + 1][1] = (vs[y][x] == '#');
    }
  }
  return cube;
}
TEST(Builder, connect_neighbor) {
  BinaryCube cube(5, 5, 3);
  cube[1][1][1] = 1; cube[2][1][1] = 1;

  cube[1][3][1] = 1; cube[2][3][1] = 1; cube[3][3][1] = 1;
  Extractor ext(cube);
  std::vector<boost::shared_ptr<Cluster> > data = ext.extract();
  Builder bld(data, 1.0, 1.0);
  bld.connect_neighbor();
  EXPECT_EQ(5, (int)bld.data_.size());
  EXPECT_EQ(1, (int)bld.data_[0]->adjacent_.size());
  EXPECT_EQ(2, (int)bld.data_[1]->adjacent_.size());
  EXPECT_EQ(1, (int)bld.data_[2]->adjacent_.size());
  EXPECT_EQ(1, (int)bld.data_[3]->adjacent_.size());
  EXPECT_EQ(1, (int)bld.data_[4]->adjacent_.size());
}
TEST(Builder, compute_gravity_point) {
  BinaryCube cube(5, 5, 3);
  cube[1][1][1] = 1; cube[2][1][1] = 1;

  cube[1][3][1] = 1; cube[2][3][1] = 1; cube[3][3][1] = 1;
  Extractor ext(cube);
  std::vector<boost::shared_ptr<Cluster> > data = ext.extract();
  Builder bld(data, 1.0, 1.0);
  bld.connect_neighbor();
  bld.compute_gravity_point();
  EXPECT_DOUBLE_EQ(1.0, bld.data_[0]->gx_);
  EXPECT_DOUBLE_EQ(3.0, bld.data_[0]->gy_);
  EXPECT_DOUBLE_EQ(1.0, bld.data_[0]->gz_);
  EXPECT_DOUBLE_EQ(2.0, bld.data_[4]->gx_);
  EXPECT_DOUBLE_EQ(1.0, bld.data_[4]->gy_);
  EXPECT_DOUBLE_EQ(1.0, bld.data_[4]->gz_);
}
TEST(Builder, convert_to_neuron_node) {
  BinaryCube cube(5, 5, 3);
  cube[1][1][1] = 1; cube[2][1][1] = 1;

  cube[1][3][1] = 1; cube[2][3][1] = 1; cube[3][3][1] = 1;
  Extractor ext(cube);
  std::vector<boost::shared_ptr<Cluster> > data = ext.extract();
  Builder bld(data, 1.0, 1.0);
  bld.connect_neighbor();
  bld.compute_gravity_point();
  bld.compute_radius();
  bld.cut_loops();
  std::vector<boost::shared_ptr<NeuronNode> > nn = bld.convert_to_neuron_node(bld.data_, bld.scale_xy_, bld.scale_z_);
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
  EXPECT_TRUE(nn[0]->has_connection(nn[1].get()));
  EXPECT_TRUE(nn[1]->has_connection(nn[0].get()));
  EXPECT_TRUE(nn[1]->has_connection(nn[2].get()));
  EXPECT_TRUE(nn[2]->has_connection(nn[1].get()));
  EXPECT_TRUE(nn[3]->has_connection(nn[4].get()));
  EXPECT_TRUE(nn[4]->has_connection(nn[3].get()));
}
TEST(Builder, convert_to_neuron) {
  BinaryCube cube(5, 5, 3);
  cube[1][1][1] = 1; cube[2][1][1] = 1;

  cube[1][3][1] = 1; cube[2][3][1] = 1; cube[3][3][1] = 1;
  Extractor ext(cube);
  std::vector<boost::shared_ptr<Cluster> > data = ext.extract();
  Builder bld(data, 1.0, 1.0);
  bld.connect_neighbor();
  bld.compute_gravity_point();
  bld.compute_radius();
  bld.cut_loops();
  std::vector<Neuron> ns = bld.convert_to_neuron(bld.data_, bld.scale_xy_, bld.scale_z_);
  EXPECT_EQ(2, (int)ns.size());
  EXPECT_EQ(3, (int)ns[0].storage_.size());
  EXPECT_EQ(2, (int)ns[1].storage_.size());
}
TEST(Builder, compute_id) {
  BinaryCube cube(5, 5, 3);
  cube[1][1][1] = 1; cube[2][1][1] = 1;

  cube[1][3][1] = 1; cube[2][3][1] = 1; cube[3][3][1] = 1;
  Extractor ext(cube);
  std::vector<boost::shared_ptr<Cluster> > data = ext.extract();
  Builder bld(data, 1.0, 1.0);
  bld.connect_neighbor();
  bld.compute_gravity_point();
  bld.compute_radius();
  bld.cut_loops();
  std::vector<Neuron> ns = bld.convert_to_neuron(bld.data_, bld.scale_xy_, bld.scale_z_);
  bld.compute_id(ns);
  EXPECT_EQ(1, ns[0].storage_[0]->id_);
  EXPECT_EQ(2, ns[0].storage_[1]->id_);
  EXPECT_EQ(3, ns[0].storage_[2]->id_);
  EXPECT_EQ(4, ns[1].storage_[0]->id_);
  EXPECT_EQ(5, ns[1].storage_[1]->id_);
}
TEST(Builder, convert_to_neuron_node_loops) {
  BinaryCube cube(5, 5, 3);
                     cube[2][1][1] = 1;
  cube[1][2][1] = 1;                    cube[3][2][1] = 1;
                     cube[2][3][1] = 1;
  Extractor ext(cube);
  std::vector<boost::shared_ptr<Cluster> > data = ext.extract();
  Builder bld(data, 1.0, 1.0);
  bld.connect_neighbor();
  bld.compute_gravity_point();
  bld.compute_radius();
  bld.cut_loops();
  std::vector<boost::shared_ptr<NeuronNode> > nn = bld.convert_to_neuron_node(bld.data_, bld.scale_xy_, bld.scale_z_);
  EXPECT_EQ(4, (int)nn.size());
  EXPECT_DOUBLE_EQ(1.0, nn[0]->gx_);
  EXPECT_DOUBLE_EQ(2.0, nn[0]->gy_);

  EXPECT_DOUBLE_EQ(2.0, nn[1]->gx_);
  EXPECT_DOUBLE_EQ(1.0, nn[1]->gy_);

  EXPECT_DOUBLE_EQ(2.0, nn[2]->gx_);
  EXPECT_DOUBLE_EQ(3.0, nn[2]->gy_);

  EXPECT_DOUBLE_EQ(3.0, nn[3]->gx_);
  EXPECT_DOUBLE_EQ(2.0, nn[3]->gy_);
  ASSERT_EQ(2, (int)nn[0]->adjacent_.size());
  ASSERT_EQ(2, (int)nn[1]->adjacent_.size());
  ASSERT_EQ(1, (int)nn[2]->adjacent_.size());
  ASSERT_EQ(1, (int)nn[3]->adjacent_.size());
  EXPECT_TRUE(nn[0]->has_connection(nn[1].get()));
  EXPECT_TRUE(nn[0]->has_connection(nn[2].get()));
  EXPECT_TRUE(nn[1]->has_connection(nn[0].get()));
  EXPECT_TRUE(nn[1]->has_connection(nn[3].get()));
  EXPECT_TRUE(nn[2]->has_connection(nn[0].get()));
  EXPECT_TRUE(nn[3]->has_connection(nn[1].get()));
}
TEST(Builder, convert_to_neuron_loops) {
  BinaryCube cube(5, 5, 3);
                     cube[2][1][1] = 1;
  cube[1][2][1] = 1;                    cube[3][2][1] = 1;
                     cube[2][3][1] = 1;
  Extractor ext(cube);
  std::vector<boost::shared_ptr<Cluster> > data = ext.extract();
  Builder bld(data, 1.0, 1.0);
  bld.connect_neighbor();
  bld.compute_gravity_point();
  bld.compute_radius();
  bld.cut_loops();
  std::vector<Neuron> ns = bld.convert_to_neuron(bld.data_, bld.scale_xy_, bld.scale_z_);
  EXPECT_EQ(1, (int)ns.size());
  EXPECT_EQ(4, (int)ns[0].storage_.size());
}
TEST(Builder, compute_id_with_loops) {
  BinaryCube cube(5, 5, 3);
                     cube[2][1][1] = 1;
  cube[1][2][1] = 1;                    cube[3][2][1] = 1;
                     cube[2][3][1] = 1;
  Extractor ext(cube);
  std::vector<boost::shared_ptr<Cluster> > data = ext.extract();
  Builder bld(data, 1.0, 1.0);
  bld.connect_neighbor();
  bld.compute_gravity_point();
  bld.compute_radius();
  bld.cut_loops();
  std::vector<Neuron> ns = bld.convert_to_neuron(bld.data_, bld.scale_xy_, bld.scale_z_);
  bld.compute_id(ns);
}
TEST(Builder, compute_node_type) {
}
