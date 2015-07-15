#include <gtest/gtest.h>
#include "writer/fileutils.h"
using namespace sigen::fileutils;
TEST(fileutils, add_extension) {
  EXPECT_EQ("hello.swc", add_extension("hello", "swc"));
  EXPECT_EQ("hello.out.swc", add_extension("hello.out", "swc"));
  EXPECT_EQ("hello.swc", add_extension("hello.swc", "swc"));
  EXPECT_EQ("hello.swc", add_extension("hello", ".swc"));
  EXPECT_EQ("hello.out.swc", add_extension("hello.out", ".swc"));
  EXPECT_EQ("hello.swc", add_extension("hello.swc", ".swc"));
}
