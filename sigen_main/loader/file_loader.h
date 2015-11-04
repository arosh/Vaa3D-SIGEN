#pragma once
#include "../common/image_sequence.h"
#include <string>
#include <vector>
namespace sigen {
class file_loader {
public:
  // load image files from specified directory
  image_sequence load(const std::string &dir_path);
};
}
