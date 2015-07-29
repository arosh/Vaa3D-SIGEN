#include "./file_loader.h"
#include <algorithm>
#include <vector>
#include <string>
#include <boost/filesystem.hpp>
#include <glog/logging.h>
// https://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Pragmas.html#Diagnostic-Pragmas
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#include <opencv2/highgui/highgui.hpp>
#pragma GCC diagnostic pop
namespace sigen {
image_sequence file_loader::load(const std::string &dir_path) {
  namespace fs = boost::filesystem;
  std::vector<std::string> fnames;
  // enumerate files in specified directory
  for (auto &&entry : fs::directory_iterator(dir_path)) {
    fnames.push_back(entry.path().string());
  }
  // sort by file name in ascending order
  std::sort(fnames.begin(), fnames.end());
  image_sequence ret;
  for (auto &&entry : fnames) {
    LOG(INFO) << entry;
    cv::Mat im = cv::imread(entry, 0 /* grayscale */);
    // ignore file if opencv cannot read it
    // (it is not image file, e.g. .txt, .DS_Store)
    if (im.data) {
      ret.push_back(im);
    }
  }
  return ret;
}
} // namespace sigen
