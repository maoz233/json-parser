/**
 * @file main.test.cc
 * @author Mao Zhang (mao.zhang233@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-05-26
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <gtest/gtest.h>

TEST(BasicTest, BasicAssertions) {
  // Expect two strings not to be equal
  EXPECT_STRNE("hello", "world");
  // Expect equality
  EXPECT_EQ(1 + 1, 2);
}