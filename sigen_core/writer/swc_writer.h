#pragma once
#include "../common/neuron.h"
#include <string>
#include <fstream>
namespace sigen {
class swc_writer {
public:
  void write(std::ostream &os, const neuron &neuron__);
  void write(const std::string &fname, const neuron &neuron__);
};
};
