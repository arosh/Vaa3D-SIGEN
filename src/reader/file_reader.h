#pragma once

#include "../common/image_sequence.h"

#include <string>
#include <vector>

namespace sigen {
class file_reader {
  std::string path_;

public:
  file_reader(const std::string &path) : path_(path) {}
  image_sequence load();
};
}
