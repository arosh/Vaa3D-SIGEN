#pragma once

#include "../common/image_sequence.h"
#include <string>
#include <vector>

namespace sigen {
class file_reader {
public:
  image_sequence load(const std::string &path);
};
}
