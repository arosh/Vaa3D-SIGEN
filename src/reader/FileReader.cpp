#include "FileReader.h"
#include <algorithm>
#include <opencv2/highgui/highgui.hpp>
#include <boost/filesystem.hpp>

namespace sigen {
cv::Mat LoadOne(const std::string &filename) {
  return cv::imread(filename);
}
ImageSequence FileReader::Load() {
  namespace fs = boost::filesystem;
  std::vector<std::string> filenames;
  for (auto &&entry : fs::directory_iterator(Path)) {
    filenames.push_back(entry.path().string());
  }
  std::sort(filenames.begin(), filenames.end());
  ImageSequence ret;
  for (auto &&entry : filenames) {
    ret.push_back(LoadOne(entry));
  }
  return ret;
}
};
