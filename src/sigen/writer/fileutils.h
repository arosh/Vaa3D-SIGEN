#pragma once
#include <string>
namespace sigen {
namespace fileutils {
// let extention = ".swc"
// add ".swc" if fname does not end with ".swc"
// hello -> hello.swc
// hello.out -> hello.out.swc
// hello.swc -> hello.swc
std::string add_extension(const std::string &fname,
                          const std::string &extention);
}; // namespace fileutils
}; // namespace sigen
