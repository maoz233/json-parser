/**
 * @file json.cc
 * @author Mao Zhang (mao.zhang233@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-05-25
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "json.h"

#include <cassert>
#include <cerrno>
#include <cmath>
#include <cstdlib>

#define EXPECT(context, character)         \
  do {                                     \
    assert(*context->json == (character)); \
    context->json++;                       \
  } while (0)

#define ISDIGIT(character) ((character) >= '0' && (character) <= '9')

#define ISDIGIT1TO9(character) ((character) >= '1' && (character) <= '9')

namespace jpp {

Result JSON::Parse(Value* value, const char* json) {
  assert(value != nullptr);

  Context context{};
  context.json = json;

  value->type = Type::Null;

  ParseWhitespace(&context);

  Result result = ParseValue(&context, value);

  if (result == Result::OK) {
    ParseWhitespace(&context);

    if (*context.json != '\0') {
      return Result::RootNotSingular;
    }
  }

  return result;
}

Result JSON::ParseValue(Context* context, Value* value) {
  switch (*context->json) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case '-':
      return ParseNumber(context, value);
    case 't':
      return ParseTrue(context, value);
    case 'f':
      return ParseFalse(context, value);
    case 'n':
      return ParseNull(context, value);
    case '\0':
      return Result::ExpectValue;
    default:
      return Result::InvalidValue;
  }
}

void JSON::ParseWhitespace(Context* context) {
  const char* p = context->json;

  while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r') {
    ++p;
  }

  context->json = p;
}

Result JSON::ParseNull(Context* context, Value* value) {
  EXPECT(context, 'n');

  if (context->json[0] != 'u' || context->json[1] != 'l' ||
      context->json[2] != 'l') {
    return Result::InvalidValue;
  }

  context->json += 3;
  value->type = Type::Null;

  return Result::OK;
}

Result JSON::ParseFalse(Context* context, Value* value) {
  EXPECT(context, 'f');

  if (context->json[0] != 'a' || context->json[1] != 'l' ||
      context->json[2] != 's' || context->json[3] != 'e') {
    return Result::InvalidValue;
  }

  context->json += 4;
  value->type = Type::False;

  return Result::OK;
}

Result JSON::ParseTrue(Context* context, Value* value) {
  EXPECT(context, 't');

  if (context->json[0] != 'r' || context->json[1] != 'u' ||
      context->json[2] != 'e') {
    return Result::InvalidValue;
  }

  context->json += 3;
  value->type = Type::True;

  return Result::OK;
}

Result JSON::ParseNumber(Context* context, Value* value) {
  // validate number
  const char* p = context->json;
  // -
  if (*p == '-') {
    ++p;
  }
  // start with 0
  if (*p == '0') {
    ++p;
  } else {
    if (!ISDIGIT1TO9(*p)) {
      return Result::InvalidValue;
    }

    do {
      ++p;
    } while (ISDIGIT(*p));
  }
  // dot
  if (*p == '.') {
    ++p;

    if (!ISDIGIT(*p)) {
      return Result::InvalidValue;
    }

    do {
      ++p;
    } while (ISDIGIT(*p));
  }
  // e or E
  if (*p == 'e' || *p == 'E') {
    ++p;

    // + or -
    if (*p == '+' || *p == '-') {
      ++p;
    }

    if (!ISDIGIT(*p)) {
      return Result::InvalidValue;
    }

    do {
      ++p;
    } while (ISDIGIT(*p));
  }

  // parse string to number
  char* end = nullptr;
  errno = 0;
  value->number = strtod(context->json, &end);
  if (context->json == end) {
    return Result::InvalidValue;
  }

  context->json = end;
  value->type = Type::Number;

  if (errno == ERANGE || value->number == HUGE_VAL) {
    return Result::NumberTooBig;
  }

  return Result::OK;
}

Type JSON::GetType(const Value* value) { return value->type; }

double JSON::GetNumber(const Value* value) {
  assert(value != nullptr && value->type == Type::Number);

  return value->number;
}

}  // namespace jpp
