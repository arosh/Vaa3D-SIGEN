#include "binary_cube.h"
namespace sigen {
std::vector<std::vector<bool>> &binary_cube::operator [](int index) {
  return data[index];
}
};
