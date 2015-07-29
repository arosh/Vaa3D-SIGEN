#pragma once
#include "../common/image_sequence.h"
#include "../common/binary_cube.h"
namespace sigen {
class binarizer {
public:
  binary_cube binarize(const image_sequence &is);
};
};
