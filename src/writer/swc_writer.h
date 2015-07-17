#pragma once
#include "../common/neuron.h"
#include <string>
#include <fstream>
namespace sigen {
class swc_writer {
  void write_rec(std::ostream &os, const Neuron &node);

public:
  void write(std::ostream &os, const Neuron &data);
  void write(const std::string &fname, const Neuron &data);
};
};
