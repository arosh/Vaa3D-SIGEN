#include <string>
#include <glog/logging.h>
#include "sigen/writer/swc_writer.h"
#include "sigen/writer/fileutils.h"
namespace sigen {
static void writeRec(std::ostream &os, const NeuronNode *node,
                     const int parent_id) {
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
  os << node->id_ << ' ' << type_id << ' ' << node->gx_ << ' ' << node->gy_
     << ' ' << node->gz_ << ' ' << node->radius_ << ' ' << parent_id
     << std::endl;
  for (NeuronNode *next : node->adjacent_) {
    if (next->id_ != parent_id) {
      writeRec(os, next, node->id_);
    }
  }
}
void SwcWriter::Write(std::ostream &os, const Neuron &neuron) {
  writeRec(os, neuron.root_, -1);
}
void SwcWriter::Write(const char *fname, const Neuron &neuron) {
  std::ofstream ofs(fname);
  writeRec(ofs, neuron.root_, -1);
}
} // namespace sigen
