#include "swc_writer.h"
#include "fileutils.h"
namespace sigen {
void swc_writer::write_rec(const neuron_node_ptr &node, std::ofstream &ofs) {
}
void swc_writer::write(const std::string &fname, const neuron &data) {
  std::ofstream ofs(fileutils::add_extension(fname, ".swc").c_str());
}
}
