#pragma once
#include "sigen/common/image_sequence.h"
#include <string>
#include <vector>
namespace sigen {
class FileLoader {
public:
  // load image files from specified directory
  image_sequence load(const std::string &dir_path);
};
}
