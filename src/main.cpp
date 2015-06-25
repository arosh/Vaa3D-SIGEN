#include <iostream>
#include <glog/logging.h>
#include "reader/file_reader.h"
#include "binarizer/binarizer.h"
#include "common/binary_cube.h"

int main() {
  google::InstallFailureSignalHandler();
  sigen::file_reader reader("/Users/arosh/ikenolab/sigen-data/ToIizuka/BMP");
  sigen::image_sequence is = reader.load();
  sigen::binarizer bin;
  sigen::binary_cube cube = bin.binarize(is);
  LOG(INFO) << "x = " << cube.x_;
  LOG(INFO) << "y = " << cube.y_;
  LOG(INFO) << "z = " << cube.z_;
}
