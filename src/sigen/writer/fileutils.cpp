#include "sigen/writer/fileutils.h"
#include <string>
#include <boost/filesystem.hpp>
#include <glog/logging.h>
namespace sigen {
namespace fileutils {
std::string add_extension(const std::string &fname,
                          const std::string &extension) {
  CHECK(!extension.empty());
  std::string ex =
      (extension.substr(0, 1) == ".") ? extension : ("." + extension);
  namespace fs = boost::filesystem;
  fs::path path(fname);
  if (path.extension() == ex)
    return path.string();
  return fname + ex;
}
} // namespace fileutils
} // namespace sigen
