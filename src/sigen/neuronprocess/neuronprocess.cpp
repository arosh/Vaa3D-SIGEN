#include "sigen/neuronprocess/neuronprocess.h"
namespace sigen {
std::vector<neuron> interpolate(const std::vector<neuron> &input) {
  std::vector<neuron> ret;
  for(int i = 0; i < (int)input.size(); ++i) {
    ret.push_back(input[i].clone());
  }
  return ret;
}
};
