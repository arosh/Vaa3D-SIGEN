#pragma once
#include "sigen/common/image_sequence.h"
#include "sigen/common/binary_cube.h"
namespace sigen {
class binarizer {
public:
  binary_cube binarize(const image_sequence &is);
};
};
