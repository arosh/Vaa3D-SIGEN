#include <iostream>
#include <glog/logging.h>
#include "./loader/file_loader.h"
#include "./binarizer/binarizer.h"
#include "./extractor/extractor.h"

// disable specified warning options
// https://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Pragmas.html#Diagnostic-Pragmas
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#include <cmdline/cmdline.h>
#pragma GCC diagnostic pop

void init_glog(const char *program_name) {
  // https://google-glog.googlecode.com/svn/trunk/doc/glog.html
  FLAGS_log_dir = "log";
  google::InitGoogleLogging(program_name);
  google::InstallFailureSignalHandler(); // print stack trace if program fault
}

int main(int argc, char *argv[]) {
  init_glog(argv[0]);
  // https://github.com/tanakh/cmdline
  cmdline::parser a;
  a.parse_check(argc, argv);

  const char *path = "/Users/arosh/ikenolab/sigen-data/ToIizuka/BMP";
  sigen::file_loader loader;
  sigen::image_sequence is = loader.load(path);
  LOG(INFO) << "len(image_sequence) = " << is.size();
  sigen::binarizer bin;
  sigen::binary_cube cube = bin.binarize(is);
  LOG(INFO) << "x = " << cube.x_;
  LOG(INFO) << "y = " << cube.y_;
  LOG(INFO) << "z = " << cube.z_;
  sigen::extractor ext(cube);
  ext.extract();
}
