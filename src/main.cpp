#include <iostream>
#include <glog/logging.h>
#include "reader/file_reader.h"
#include "binarizer/binarizer.h"
#include "common/binary_cube.h"

// https://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Pragmas.html#Diagnostic-Pragmas
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#include "cmdline/cmdline.h"
#pragma GCC diagnostic pop

int main(int argc, char *argv[]) {
  // https://google-glog.googlecode.com/svn/trunk/doc/glog.html
  FLAGS_log_dir = "log";
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();

  cmdline::parser a;
  a.parse_check(argc, argv);

  sigen::file_reader reader("/Users/arosh/ikenolab/sigen-data/ToIizuka/BMP");
  sigen::image_sequence is = reader.load();
  sigen::binarizer bin;
  sigen::binary_cube cube = bin.binarize(is);
  LOG(INFO) << "x = " << cube.x_;
  LOG(INFO) << "y = " << cube.y_;
  LOG(INFO) << "z = " << cube.z_;
}
