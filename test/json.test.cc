/**
 * @file json.test.cc
 * @author Mao Zhang (mao.zhang233@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-05-29
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <gtest/gtest.h>

#define INCLUDE_JPP_JSON
#include "json.h"

TEST(jpp_json_test, BasicAssertions) {
  jpp::Value value{};
  value.type = jpp::Type::True;

  EXPECT_EQ(jpp::Result::OK, jpp::JSON::Parse(&value, "null"));
  EXPECT_EQ(jpp::Type::Null, jpp::JSON::GetType(&value));
}
