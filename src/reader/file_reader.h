#include <string>
#include <vector>
#include "image_sequence.h"

namespace sigen {
class file_reader {
  std::string path_;
public:
  file_reader(const std::string &path) : path_(path) {}
  image_sequence load();
};
}
