#include "swc_writer.h"
#include "fileutils.h"
#include <glog/logging.h>
namespace sigen {
void swc_writer::write_rec(std::ostream &os, const Neuron &node) {
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
  CHECK(type_id != -1);
  int parent_id = (node->parent_.expired() ? -1 : node->parent_.lock()->id_);
  os << node->id_ << ' '
      << type_id   << ' '
      << node->gx_ << ' '
      << node->gy_ << ' '
      << node->gz_ << ' '
      << node->radius_ << ' '
      << parent_id << std::endl;
  for (auto next__ : node->childs_) {
    auto next = next__.lock();
    CHECK(next != node->parent_.lock());
    write_rec(os, next);
  }
}
void swc_writer::write(std::ostream &os, const Neuron &data) {
  write_rec(os, data);
}
void swc_writer::write(const std::string &fname, const Neuron &data) {
  std::ofstream ofs(fileutils::add_extension(fname, ".swc").c_str());
  write(ofs, data);
}
}
