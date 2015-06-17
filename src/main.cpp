#include <iostream>
#include "hello.h"
#include "reader/ImageSequence.h"

int main() {
  sigen::ImageSequence is;
  std::cout << "Hello, world!" << std::endl;
  std::cout << "add(1, 2) = " << add(1, 2) << std::endl;
}
