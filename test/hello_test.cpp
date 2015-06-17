#include <gtest/gtest.h>
#include "hello.h"

TEST(hello, helloAdd) {
  EXPECT_EQ(5, add(2, 3));
}
