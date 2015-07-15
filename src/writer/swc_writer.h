#pragma once
#include "../common/neuron.h"
#include <string>
namespace sigen {
class swc_writer {
public:
  void write(const std::string &fname, const neuron &data);
};
};
