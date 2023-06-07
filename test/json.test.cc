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

TEST(JSONParseTest, ParseNull) {
  jpp::Value value{};
  value.type = jpp::Type::True;

  EXPECT_EQ(jpp::Result::OK, jpp::JSON::Parse(&value, "null"));
  EXPECT_EQ(jpp::Result::ExpectValue, jpp::JSON::Parse(&value, ""));
  EXPECT_EQ(jpp::Result::ExpectValue, jpp::JSON::Parse(&value, " "));
  EXPECT_EQ(jpp::Result::InvalidValue, jpp::JSON::Parse(&value, "nul"));
  EXPECT_EQ(jpp::Result::RootNotSingular, jpp::JSON::Parse(&value, "null x"));
  EXPECT_EQ(jpp::Type::Null, jpp::JSON::GetType(&value));
}

TEST(JSONParseTest, ParseFalse) {
  jpp::Value value{};
  value.type = jpp::Type::Null;

  EXPECT_EQ(jpp::Result::OK, jpp::JSON::Parse(&value, "false"));
  EXPECT_EQ(jpp::Type::False, jpp::JSON::GetType(&value));
}

TEST(JSONParseTest, ParseTrue) {
  jpp::Value value{};
  value.type = jpp::Type::Null;

  EXPECT_EQ(jpp::Result::OK, jpp::JSON::Parse(&value, "true"));
  EXPECT_EQ(jpp::Type::True, jpp::JSON::GetType(&value));
}

TEST(JSONParseTest, ParseNumber) {
  jpp::Value value{};
  value.type = jpp::Type::Null;

  EXPECT_EQ(jpp::Result::OK, jpp::JSON::Parse(&value, "0"));
  EXPECT_EQ(jpp::Type::Number, jpp::JSON::GetType(&value));
  EXPECT_DOUBLE_EQ(0.0, jpp::JSON::GetNumber(&value));

  EXPECT_EQ(jpp::Result::OK, jpp::JSON::Parse(&value, "-0"));
  EXPECT_EQ(jpp::Type::Number, jpp::JSON::GetType(&value));
  EXPECT_DOUBLE_EQ(0.0, jpp::JSON::GetNumber(&value));

  EXPECT_EQ(jpp::Result::OK, jpp::JSON::Parse(&value, "-0.0"));
  EXPECT_EQ(jpp::Type::Number, jpp::JSON::GetType(&value));
  EXPECT_DOUBLE_EQ(0.0, jpp::JSON::GetNumber(&value));

  EXPECT_EQ(jpp::Result::OK, jpp::JSON::Parse(&value, "1"));
  EXPECT_EQ(jpp::Type::Number, jpp::JSON::GetType(&value));
  EXPECT_DOUBLE_EQ(1.0, jpp::JSON::GetNumber(&value));

  EXPECT_EQ(jpp::Result::OK, jpp::JSON::Parse(&value, "-1"));
  EXPECT_EQ(jpp::Type::Number, jpp::JSON::GetType(&value));
  EXPECT_DOUBLE_EQ(-1.0, jpp::JSON::GetNumber(&value));

  EXPECT_EQ(jpp::Result::OK, jpp::JSON::Parse(&value, "1.5"));
  EXPECT_EQ(jpp::Type::Number, jpp::JSON::GetType(&value));
  EXPECT_DOUBLE_EQ(1.5, jpp::JSON::GetNumber(&value));

  EXPECT_EQ(jpp::Result::OK, jpp::JSON::Parse(&value, "-1.5"));
  EXPECT_EQ(jpp::Type::Number, jpp::JSON::GetType(&value));
  EXPECT_DOUBLE_EQ(-1.5, jpp::JSON::GetNumber(&value));

  EXPECT_EQ(jpp::Result::OK, jpp::JSON::Parse(&value, "3.1416"));
  EXPECT_EQ(jpp::Type::Number, jpp::JSON::GetType(&value));
  EXPECT_DOUBLE_EQ(3.1416, jpp::JSON::GetNumber(&value));

  EXPECT_EQ(jpp::Result::OK, jpp::JSON::Parse(&value, "-3.1416"));
  EXPECT_EQ(jpp::Type::Number, jpp::JSON::GetType(&value));
  EXPECT_DOUBLE_EQ(-3.1416, jpp::JSON::GetNumber(&value));

  EXPECT_EQ(jpp::Result::OK, jpp::JSON::Parse(&value, "1E10"));
  EXPECT_EQ(jpp::Type::Number, jpp::JSON::GetType(&value));
  EXPECT_DOUBLE_EQ(1E10, jpp::JSON::GetNumber(&value));

  EXPECT_EQ(jpp::Result::OK, jpp::JSON::Parse(&value, "1e10"));
  EXPECT_EQ(jpp::Type::Number, jpp::JSON::GetType(&value));
  EXPECT_DOUBLE_EQ(1e10, jpp::JSON::GetNumber(&value));

  EXPECT_EQ(jpp::Result::OK, jpp::JSON::Parse(&value, "-1E10"));
  EXPECT_EQ(jpp::Type::Number, jpp::JSON::GetType(&value));
  EXPECT_DOUBLE_EQ(-1E10, jpp::JSON::GetNumber(&value));

  EXPECT_EQ(jpp::Result::OK, jpp::JSON::Parse(&value, "-1e10"));
  EXPECT_EQ(jpp::Type::Number, jpp::JSON::GetType(&value));
  EXPECT_DOUBLE_EQ(-1e10, jpp::JSON::GetNumber(&value));

  EXPECT_EQ(jpp::Result::OK, jpp::JSON::Parse(&value, "-1E+10"));
  EXPECT_EQ(jpp::Type::Number, jpp::JSON::GetType(&value));
  EXPECT_DOUBLE_EQ(-1E+10, jpp::JSON::GetNumber(&value));

  EXPECT_EQ(jpp::Result::OK, jpp::JSON::Parse(&value, "-1E-10"));
  EXPECT_EQ(jpp::Type::Number, jpp::JSON::GetType(&value));
  EXPECT_DOUBLE_EQ(-1E-10, jpp::JSON::GetNumber(&value));

  EXPECT_EQ(jpp::Result::OK, jpp::JSON::Parse(&value, "1.234E+10"));
  EXPECT_EQ(jpp::Type::Number, jpp::JSON::GetType(&value));
  EXPECT_DOUBLE_EQ(1.234E+10, jpp::JSON::GetNumber(&value));

  EXPECT_EQ(jpp::Result::OK, jpp::JSON::Parse(&value, "1.234E-10"));
  EXPECT_EQ(jpp::Type::Number, jpp::JSON::GetType(&value));
  EXPECT_DOUBLE_EQ(1.234E-10, jpp::JSON::GetNumber(&value));

  EXPECT_EQ(jpp::Result::OK, jpp::JSON::Parse(&value, "1.0000000000000002"));
  EXPECT_EQ(jpp::Type::Number, jpp::JSON::GetType(&value));
  EXPECT_DOUBLE_EQ(1.0000000000000002, jpp::JSON::GetNumber(&value));

  EXPECT_EQ(jpp::Result::OK, jpp::JSON::Parse(&value, "1.0000000000000002"));
  EXPECT_EQ(jpp::Type::Number, jpp::JSON::GetType(&value));
  EXPECT_DOUBLE_EQ(1.0000000000000002, jpp::JSON::GetNumber(&value));

  EXPECT_EQ(jpp::Result::OK,
            jpp::JSON::Parse(&value, "2.2250738585072014e-308"));
  EXPECT_EQ(jpp::Type::Number, jpp::JSON::GetType(&value));
  EXPECT_DOUBLE_EQ(2.2250738585072014e-308, jpp::JSON::GetNumber(&value));

  EXPECT_EQ(jpp::Result::OK,
            jpp::JSON::Parse(&value, "1.7976931348623157e+308"));
  EXPECT_EQ(jpp::Type::Number, jpp::JSON::GetType(&value));
  EXPECT_DOUBLE_EQ(1.7976931348623157e+308, jpp::JSON::GetNumber(&value));

  EXPECT_EQ(jpp::Result::NumberTooBig, jpp::JSON::Parse(&value, "1e-10000"));
  EXPECT_EQ(jpp::Type::Number, jpp::JSON::GetType(&value));
  EXPECT_DOUBLE_EQ(0.0, jpp::JSON::GetNumber(&value));

  EXPECT_EQ(jpp::Result::NumberTooBig,
            jpp::JSON::Parse(&value, "4.9406564584124654e-324"));
  EXPECT_EQ(jpp::Type::Number, jpp::JSON::GetType(&value));
  EXPECT_DOUBLE_EQ(4.9406564584124654e-324, jpp::JSON::GetNumber(&value));

  EXPECT_EQ(jpp::Result::NumberTooBig,
            jpp::JSON::Parse(&value, "2.2250738585072009e-308"));
  EXPECT_EQ(jpp::Type::Number, jpp::JSON::GetType(&value));
  EXPECT_DOUBLE_EQ(2.2250738585072009e-308, jpp::JSON::GetNumber(&value));

  EXPECT_EQ(jpp::Result::InvalidValue, jpp::JSON::Parse(&value, "+0"));
  EXPECT_EQ(jpp::Result::InvalidValue, jpp::JSON::Parse(&value, ".123"));
  EXPECT_EQ(jpp::Result::InvalidValue, jpp::JSON::Parse(&value, "1."));
  EXPECT_EQ(jpp::Result::InvalidValue, jpp::JSON::Parse(&value, "INF"));
  EXPECT_EQ(jpp::Result::InvalidValue, jpp::JSON::Parse(&value, "inf"));
  EXPECT_EQ(jpp::Result::InvalidValue, jpp::JSON::Parse(&value, "NAN"));
  EXPECT_EQ(jpp::Result::InvalidValue, jpp::JSON::Parse(&value, "nan"));
}
