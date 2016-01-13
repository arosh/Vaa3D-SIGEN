#include <iostream>
#include <vector>
#include <string>
#include <glog/logging.h>
#include "sigen/loader/file_loader.h"
#include "sigen/binarizer/binarizer.h"
#include "sigen/extractor/extractor.h"
#include "sigen/builder/builder.h"
#include "sigen/writer/swc_writer.h"
#include "sigen/neuronprocess/neuronprocess.h"

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
  a.add<double>("dt", '\0', "distance threshold", false, 0.0);
  a.add<int>("vt", '\0', "volume threshold", false, 0);
  a.add<int>("clipping", '\0', "clipping level", false, 0);
  a.add<int>("smoothing", '\0', "smoothing level", false, 0);
  a.parse_check(argc, argv);

  sigen::FileLoader loader;
  sigen::ImageSequence is = loader.load(a.get<std::string>("input"));
  LOG(INFO) << "load (done)";
  sigen::Binarizer bin;
  sigen::BinaryCube cube = bin.binarize(is);
  LOG(INFO) << "binarize (done)";
  is.clear();
  sigen::Extractor ext(cube);
  std::vector<boost::shared_ptr<sigen::Cluster> > clusters = ext.extract();
  LOG(INFO) << "extract (done)";
  cube.clear();
  sigen::Builder builder(clusters, a.get<double>("scale-xy"), a.get<double>("scale-z"));
  std::vector<sigen::Neuron> ns = builder.build();
  LOG(INFO) << "build (done)";
  ns = sigen::interpolate(ns, a.get<double>("dt"), a.get<int>("vt"));
  LOG(INFO) << "interpolate (done)";
  sigen::SwcWriter writer;
  for (int i = 0; i < (int)ns.size(); ++i) {
    std::string filename =
        "sample_output/" + std::to_string(i) + ".swc";
    writer.write(filename, ns[i]);
  }
  LOG(INFO) << "write (done)";
}
