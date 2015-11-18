#include <iostream>
#include <glog/logging.h>
#include "loader/file_loader.h"
#include "binarizer/binarizer.h"
#include "extractor/extractor.h"
#include "builder/builder.h"
#include "writer/swc_writer.h"

// disable specified warning options
// https://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Pragmas.html#Diagnostic-Pragmas
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#include <cmdline/cmdline.h>
#pragma GCC diagnostic pop

void init_glog(const char *program_name) {
  // https://google-glog.googlecode.com/svn/trunk/doc/glog.html
  // FLAGS_log_dir = "log";
  FLAGS_logtostderr = true;
  google::InitGoogleLogging(program_name);
  google::InstallFailureSignalHandler(); // print stack trace if program fault
}

int main(int argc, char *argv[]) {
  init_glog(argv[0]);

  cmdline::parser a;
  a.add<std::string>("input", 'i', "input image directory");
  a.add<std::string>("output", 'o', "output filename");
  a.add<double>("scale-xy", '\0', "", false, 1.0);
  a.add<double>("scale-z", '\0', "", false, 1.0);
  a.parse_check(argc, argv);

  sigen::file_loader loader;
  sigen::image_sequence is = loader.load(a.get<std::string>("input"));
  LOG(INFO) << "load (done)";
  sigen::binarizer bin;
  sigen::binary_cube cube = bin.binarize(is);
  LOG(INFO) << "binarize (done)";
  is.clear();
  sigen::extractor ext(cube);
  std::vector<boost::shared_ptr<sigen::cluster> > clusters = ext.extract();
  LOG(INFO) << "extract (done)";
  cube.clear();
  sigen::builder builder(clusters, a.get<double>("scale-xy"),
                         a.get<double>("scale-z"));
  std::vector<sigen::neuron> ns = builder.build();
  LOG(INFO) << "build (done)";
  sigen::swc_writer writer;
  for (int i = 0; i < (int)ns.size(); ++i) {
    std::string filename =
        "sample_output/" + std::to_string(i) + ".swc";
    writer.write(filename, ns[i]);
  }
  LOG(INFO) << "write (done)";
}
