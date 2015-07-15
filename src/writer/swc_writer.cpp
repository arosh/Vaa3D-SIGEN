#include "swc_writer.h"
#include "fileutils.h"
#include <glog/logging.h>
namespace sigen {
void swc_writer::write_rec(const neuron_node_ptr &node, std::ofstream &ofs) {
  int type_id = -1;
  switch(node->type_) {
  case neuron_node_type::EDGE:
    type_id = 6;
    break;
  case neuron_node_type::BRANCH:
    type_id = 5;
    break;
  case neuron_node_type::CONNECT:
    type_id = 3;
    break;
  }
  CHECK(type_id != -1);
  int parent_id = (node->parent_ == nullptr ? -1 : node->parent_->id_);
  ofs << node->id_ << ' '
      << type_id << ' '
      << node->gx_ << ' '
      << node->gy_ << ' '
      << node->gz_ << ' '
      << node->radius_ << ' '
      << parent_id << std::endl;
  for (const auto &next : node->adjacent_) {
    if (next != node->parent_) write_rec(next, ofs);
  }
}
void swc_writer::write(const std::string &fname, const neuron &data) {
  std::ofstream ofs(fileutils::add_extension(fname, ".swc").c_str());
  write_rec(data.root_, ofs);
}
}
