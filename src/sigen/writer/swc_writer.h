#pragma once
#include "sigen/common/neuron.h"
#include <string>
#include <fstream>
namespace sigen {
class SwcWriter {
public:
  void Write(std::ostream &os, const Neuron &neuron);
  void Write(const char *fname, const Neuron &neuron);
};
} // sigen
