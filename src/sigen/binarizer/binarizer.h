#pragma once
#include "sigen/common/image_sequence.h"
#include "sigen/common/binary_cube.h"
namespace sigen {
class Binarizer {
public:
  BinaryCube Binarize(const ImageSequence &is);
};
}
