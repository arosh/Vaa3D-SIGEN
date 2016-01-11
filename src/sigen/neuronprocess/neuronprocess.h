#pragma once
#include "sigen/common/neuron.h"
namespace sigen {
std::vector<Neuron> interpolate(const std::vector<Neuron> &input, const double dt, const int vt);
std::vector<Neuron> smoothing(const std::vector<Neuron> &input, const int n_iter);
std::vector<Neuron> clipping(const std::vector<Neuron> &input, const int level);
};
