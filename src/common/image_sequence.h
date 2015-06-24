#pragma once

// https://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Pragmas.html#Diagnostic-Pragmas
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"

#include <vector>
#include <opencv2/core/core.hpp>

#pragma GCC diagnostic pop

namespace sigen {
typedef std::vector<cv::Mat> image_sequence;
}
