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

void init_glog(const char *program_name) {
  // https://google-glog.googlecode.com/svn/trunk/doc/glog.html
  FLAGS_log_dir = "log";
  google::InitGoogleLogging(program_name);
  google::InstallFailureSignalHandler();
}

int main(int argc, char *argv[]) {
  init_glog(argv[0]);
  cmdline::parser a;
  a.parse_check(argc, argv);

  const char *path = "/Users/arosh/ikenolab/sigen-data/ToIizuka/BMP";
  sigen::file_reader reader;
  sigen::image_sequence is = reader.load(path);
  sigen::binarizer bin;
  sigen::binary_cube cube = bin.binarize(is);
  LOG(INFO) << "x = " << cube.x_;
  LOG(INFO) << "y = " << cube.y_;
  LOG(INFO) << "z = " << cube.z_;
}
