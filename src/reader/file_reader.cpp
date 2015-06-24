#include "file_reader.h"
#include <algorithm>
#include <opencv2/highgui/highgui.hpp>
#include <boost/filesystem.hpp>
#include <glog/logging.h>

namespace sigen {
image_sequence file_reader::load() {
  namespace fs = boost::filesystem;
  std::vector<std::string> fnames;
  for (auto &&entry : fs::directory_iterator(path_)) {
    fnames.push_back(entry.path().string());
  }
  std::sort(fnames.begin(), fnames.end());
  image_sequence ret;
  for (auto &&entry : fnames) {
    LOG(INFO) << entry;
    cv::Mat im = cv::imread(entry);
    if(im.data) {
      ret.push_back(im);
    }
  }
  return ret;
}
};
