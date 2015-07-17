#include <gtest/gtest.h>
#include <memory>
#include <utility>
#include <vector>
TEST(shared_ptr, nullptr) {
  std::shared_ptr<int> p;
  EXPECT_EQ(p, nullptr);
  EXPECT_FALSE((bool)p);
}
TEST(shared_ptr, equal) {
  std::shared_ptr<int> p = std::make_shared<int>(334);
  std::shared_ptr<int> q = p;
  EXPECT_EQ(p, q);
}
TEST(shared_ptr, ref_count) {
  std::shared_ptr<int> a;
  {
    std::shared_ptr<int> b = std::make_shared<int>(334);
    a = b;
  }
  EXPECT_TRUE((bool)a);
}
TEST(weak_ptr, ref_count) {
  std::weak_ptr<int> a;
  {
    a = std::make_shared<int>(334);
  }
  EXPECT_TRUE(a.expired());
}
struct S {
  int value_;
  std::weak_ptr<S> next_;
  S(int value, std::weak_ptr<S> next) : value_(value), next_(next) {}
  S(int value) : value_(value) {}
};
TEST(cyclic_ptr, release) {
  std::weak_ptr<S> p, q;
  {
    std::vector<std::shared_ptr<S>> owner;
    owner.push_back(std::make_shared<S>(123));
    owner.push_back(std::make_shared<S>(456));
    owner[0]->next_ = owner[1];
    owner[1]->next_ = owner[0];
    p = owner[0];
    q = owner[1];
    EXPECT_EQ(456, p.lock()->next_.lock()->value_);
    EXPECT_EQ(123, q.lock()->next_.lock()->value_);
  }
  EXPECT_TRUE(p.expired());
  EXPECT_TRUE(q.expired());
}
