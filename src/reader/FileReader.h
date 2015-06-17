#include <string>
#include <vector>
#include "ImageSequence.h"

namespace sigen {
class FileReader {
public:
  const std::string &Path;
  FileReader(const std::string &path) : Path(path) { }
  ImageSequence Load();
};
}
