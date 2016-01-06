#pragma once
#include "sigen/common/neuron.h"
namespace sigen {
std::vector<neuron> interpolate(const std::vector<neuron> &input, const double dt, const int vt);
std::vector<neuron> smoothing(const std::vector<neuron> &input, const int n_iter);
std::vector<neuron> clipping(const std::vector<neuron> &input, const int level);
};
