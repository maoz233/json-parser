/**
 * @file json.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-06-27
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef JSON_PARSER_INCLUDE_JSON_H_
#define JSON_PARSER_INCLUDE_JSON_H_

#include <cstdlib>

namespace jpp {

enum class Type { Null, False, True, Number, String, Array, Object };

struct String {
  char* literal;
  std::size_t length;
};

struct Value {
  union {
    bool boolean;
    double number;
    String string;
  };
  Type type;
};

enum class Result {
  OK,
  ExpectValue,
  InvalidValue,
  RootNotSingular,
  NumberTooBig,
  MissingQuotationMark,
  InvalidStringEscape,
  InvalidStringCharacter
};

struct Context {
  const char* json;
  char* stack;
  std::size_t top;
  std::size_t size;
};

class JSON {
 public:
  JSON() = default;
  ~JSON() = default;

  /**
   *  @brief parse JSON object from string literal: JSON-text = ws value ws
   *
   *  @param value
   *  @param json
   *  @return Result
   */
  static Result Parse(Value* value, const char* json);

  /**
   *  @brief
   *
   *  @param context
   *  @param value
   *  @return Result
   */
  static Result ParseValue(Context* context, Value* value);

  /**
   *  @brief ws =* (%x20 / %x09 / %x0A / %x0D)
   *
   *  @param context
   */
  static void ParseWhitespace(Context* context);

  /**
   *  @brief null = "null"
   *
   *  @param context
   *  @param value
   */
  static Result ParseNull(Context* context, Value* value);

  /**
   *  @brief false = "false"
   *
   *  @param context
   *  @param value
   */
  static Result ParseFalse(Context* context, Value* value);

  /**
   *  @brief true = "true"
   *
   *  @param context
   *  @param value
   */
  static Result ParseTrue(Context* context, Value* value);

  /**
  *  @brief number = [ "-" ] int [ frac ] [ exp ]
            int = "0" / digit1-9* digit
            frac = "." 1*digit
            exp = ("e" / "E") ["-" / "+"] 1*digit
  *
  *  @param context
  *  @param value
  *  @return Result
  */
  static Result ParseNumber(Context* context, Value* value);

  /**
  *  @brief string = quotation-mark* char quotation-mark
            char = unescaped /
              escape (
                  %x22 /          ; "    quotation mark  U+0022
                  %x5C /          ; \    reverse solidus U+005C
                  %x2F /          ; /    solidus         U+002F
                  %x62 /          ; b    backspace       U+0008
                  %x66 /          ; f    form feed       U+000C
                  %x6E /          ; n    line feed       U+000A
                  %x72 /          ; r    carriage return U+000D
                  %x74 /          ; t    tab             U+0009
                  %x75 4HEXDIG )  ; uXXXX                U+XXXX
            escape = %x5C          ; \
            quotation-mark = %x22  ; "
            unescaped = %x20-21 / %x23-5B / %x5D-10FFFF
  *
  *  @param context
  *  @param value
  *  @return Result
  */
  static Result ParseString(Context* context, Value* value);

  /**
   *  @brief Push into context stack
   *
   *  @param context
   *  @param size
   *  @return void*
   */
  static void* ContextPush(Context* context, size_t size);

  /**
   *  @brief Pop out context stack
   *
   *  @param context
   *  @param size
   *  @return void*
   */
  static void* ContextPop(Context* context, size_t size);

  /**
   * @brief Init value
   *
   * @param value
   * @return void*
   */
  static void InitValue(Value* value);

  /**
   * @brief Clear value
   *
   * @param value
   */
  static void FreeValue(Value* value);

  /**
   *  @brief Get the Type of value
   *
   *  @param value
   *  @return Type
   */
  static Type GetType(const Value* value);

  /**
   *  @brief clear value
   *
   *  @param value
   */
  static void SetNull(Value* value);

  /**
   *  @brief Get the Boolean(True/False) from value
   *
   *  @param value
   *  @return bool
   */
  static bool GetBoolean(const Value* value);

  /**
   * @brief Set the Boolean object
   *
   * @param value
   * @param boolean
   */
  static void SetBoolean(Value* value, bool boolean);

  /**
   *  @brief Get the Number from value
   *
   *  @param value
   *  @return double
   */
  static double GetNumber(const Value* value);

  /**
   * @brief Set the Number object
   *
   * @param value
   * @param number
   */
  static void SetNumber(Value* value, double number);

  /**
   *  @brief Get the String from value
   *
   *  @param value
   *  @return String
   */
  static const char* GetString(const Value* value);

  /**
   *  @brief Get length of String in value
   *
   *  @param value
   *  @return std::size_t
   */
  static std::size_t GetStringLength(const Value* value);

  /**
   *  @brief Set the String value
   *
   *  @param value
   *  @param str
   *  @param length
   */
  static void SetString(Value* value, const char* str, std::size_t length);

 private:
};

}  // namespace jpp

#endif  // JSON_PARSER_INCLUDE_JSON_H_
