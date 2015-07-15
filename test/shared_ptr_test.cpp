#include <gtest/gtest.h>
#include <memory>
TEST(shared_ptr, nullptr) {
  std::shared_ptr<int> p;
  EXPECT_EQ(p, nullptr);
}
TEST(shared_ptr, equal) {
  std::shared_ptr<int> p = std::make_shared<int>(334);
  std::shared_ptr<int> q = p;
  EXPECT_EQ(p, q);
}
