#pragma once
#include "../common/neuron.h"
#include <string>
#include <fstream>
namespace sigen {
class swc_writer {
  void write_rec(const neuron_node_ptr &node, std::ofstream &ofs);
public:
  void write(const std::string &fname, const neuron &data);
};
};
