#include "binarizer.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <glog/logging.h>

namespace sigen {
void binarizer::binarize(image_sequence &is) {
  for (auto &&image : is) {
    CHECK_EQ(image.dims, 1) << "image.dim != 1";
    cv::threshold(image, image, /* thresh = */ 127, /* maxval = */ 255, cv::THRESH_BINARY);
  }
}
}
