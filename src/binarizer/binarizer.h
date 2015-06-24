#pragma once
#include "../reader/image_sequence.h"
#include "binary_cube.h"
namespace sigen {
class binarizer {
public:
  binary_cube binarize(image_sequence &is);
};
};
