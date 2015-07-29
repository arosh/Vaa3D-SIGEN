#include "./swc_writer.h"
#include "./fileutils.h"
#include <glog/logging.h>
#include <string>
namespace sigen {
void swc_writer::write_rec(std::ostream &os, const neuron_node *node) {
  int type_id = -1;
  switch (node->type_) {
  case neuron_type::EDGE:
    type_id = 6;
    break;
  case neuron_type::BRANCH:
    type_id = 5;
    break;
  case neuron_type::CONNECT:
    type_id = 3;
    break;
  }
  CHECK_NE(-1, type_id);
  int parent_id = (node->parent_ == nullptr ? -1 : node->parent_->id_);
  os << node->id_ << ' '
     << type_id << ' '
     << node->gx_ << ' '
     << node->gy_ << ' '
     << node->gz_ << ' '
     << node->radius_ << ' '
     << parent_id << std::endl;
  for (auto next : node->adjacent_) {
    if (next != node->parent_) {
      write_rec(os, next);
    }
  }
}
void swc_writer::write(std::ostream &os, const neuron &neuron__) {
  write_rec(os, neuron__.root_);
}
void swc_writer::write(const std::string &fname, const neuron &neuron__) {
  std::ofstream ofs(fileutils::add_extension(fname, ".swc").c_str());
  write(ofs, neuron__);
}
} // namespace sigen
