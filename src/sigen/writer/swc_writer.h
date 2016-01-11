#pragma once
#include "sigen/common/neuron.h"
#include <string>
#include <fstream>
namespace sigen {
class SwcWriter {
public:
  void write(std::ostream &os, const Neuron &neuron__);
  void write(const std::string &fname, const Neuron &neuron__);
};
};
