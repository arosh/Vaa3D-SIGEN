#pragma once
#include "sigen/common/neuron.h"
#include <string>
#include <fstream>
namespace sigen {
class SwcWriter {
public:
  void write(std::ostream &os, const Neuron &neuron);
  void write(const char *fname, const Neuron &neuron);
};
};
