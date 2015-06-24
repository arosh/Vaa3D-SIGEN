#include <iostream>
#include <glog/logging.h>
#include "hello.h"
#include "reader/file_reader.h"

int main() {
  google::InstallFailureSignalHandler();
  sigen::file_reader reader("/Users/arosh/ikenolab/sigen-data/ToIizuka/BMP");
  sigen::image_sequence is = reader.load();
  for (auto &&entry : is) {
    std::cout << entry.cols << 'x' << entry.rows << std::endl;
  }
}
