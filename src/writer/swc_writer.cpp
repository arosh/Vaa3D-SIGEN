#include "./swc_writer.h"
#include "./fileutils.h"
#include <glog/logging.h>
#include <string>
namespace sigen {
static void write_rec(std::ostream &os, const neuron_node *node, const int parent_id) {
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
  os << node->id_ << ' '
     << type_id << ' '
     << node->gx_ << ' '
     << node->gy_ << ' '
     << node->gz_ << ' '
     << node->radius_ << ' '
     << parent_id << std::endl;
  for (auto next : node->adjacent_) {
    if (next->id_ != parent_id) {
      write_rec(os, next, node->id_);
    }
  }
}
void swc_writer::write(std::ostream &os, const neuron &neu) {
  write_rec(os, neu.root_, -1);
}
void swc_writer::write(const std::string &fname, const neuron &neu) {
  std::ofstream ofs(fileutils::add_extension(fname, ".swc").c_str());
  write_rec(ofs, neu.root_, -1);
}
} // namespace sigen
