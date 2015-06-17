#include "FileReader.h"
#include <algorithm>
#include <opencv2/highgui/highgui.hpp>
#include <boost/filesystem.hpp>

namespace sigen {
ImageSequence FileReader::Load() {
  using fs = boost::filesystem;
  std::vector<std::string> filenames;
  for (auto &&p : fs::directory_iterator(Path)) {
    filenames.push_back(p.string());
  }
  std::sort(filenames.begin(), filenames.end());
  ImageSequence ret;
  for (auto &&p : filenames) {
    ret.push_back(LoadOne(p));
  }
  return ret;
}

cv::Mat LoadOne(const std::string &filename) {
  return cv::imread(filename);
}
};
