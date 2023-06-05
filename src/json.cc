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

#define EXPECT(context, character)         \
  do {                                     \
    assert(*context->json == (character)); \
    context->json++;                       \
  } while (0)

namespace jpp {

Result JSON::Parse(Value* value, const char* json) {
  assert(value != nullptr);

  Context context{};
  context.json = json;

  value->type = Type::Null;

  ParseWhitespace(&context);

  return ParseValue(&context, value);
}

Result JSON::ParseValue(Context* context, Value* value) {
  switch (*context->json) {
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

Type JSON::GetType(const Value* value) { return value->type; }

}  // namespace jpp
