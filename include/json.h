/**
 * @file json.h
 * @author Mao Zhang (mao.zhang233@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-05-25
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef JSON_PARSER_INCLUDE_JSON_H_
#define JSON_PARSER_INCLUDE_JSON_H_

namespace jpp {

enum class Type { Null, False, True, Number, String, Array, Object };

struct Value {
  Type type;
};

enum class Result { OK, ExpectValue, InvalidValue, RootNotSingular };

struct Context {
  const char* json;
};

class JSON {
 public:
  JSON() = default;
  ~JSON() = default;

  /**
   * @brief parse JSON object from string literal: JSON-text = ws value ws
   *
   * @param value
   * @param json
   * @return Result
   */
  static Result Parse(Value* value, const char* json);

  /**
   * @brief
   *
   * @param context
   * @param value
   * @return Result
   */
  static Result ParseValue(Context* context, Value* value);

  /**
   * @brief ws = *(%x20 / %x09 / %x0A / %x0D)
   *
   * @param context
   */
  static void ParseWhitespace(Context* context);

  /**
   * @brief null  = "null"
   *
   * @param context
   * @param value
   */
  static Result ParseNull(Context* context, Value* value);

  /**
   * @brief Get the Type of value
   *
   * @param value
   * @return Type
   */
  static Type GetType(const Value* value);

 private:
};

}  // namespace jpp

#endif  // JSON_PARSER_INCLUDE_JSON_H_