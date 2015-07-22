#pragma once
#include "../common/neuron.h"
#include <string>
#include <fstream>
namespace sigen {
class swc_writer {
  void write_rec(std::ostream &os, const neuron_part *node);

public:
  void write(std::ostream &os, const neuron_part *root);
  void write(const std::string &fname, const neuron_part *root);
};
};
