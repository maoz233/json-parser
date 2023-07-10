/**
 * @file json.cc
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-06-27
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "json.h"

#include <cassert>
#include <cerrno>
#include <cmath>
#include <cstdlib>
#include <cstring>

namespace jpp {

#ifndef JPP_STACK_INIT_SIZE
#define JPP_STACK_INIT_SIZE 256
#endif

#define EXPECT(context, character)         \
  do {                                     \
    assert(*context->json == (character)); \
    context->json++;                       \
  } while (0)

#define ISDIGIT(character) ((character) >= '0' && (character) <= '9')

#define ISDIGIT1TO9(character) ((character) >= '1' && (character) <= '9')

#define PUTCHAR(context, character)                                      \
  do {                                                                   \
    *reinterpret_cast<char*>(ContextPush(context, sizeof(character))) == \
        (character);                                                     \
  } while (0)

Result JSON::Parse(Value* value, const char* json) {
  assert(value != nullptr);

  Context context{};
  context.json = json;
  context.stack = nullptr;
  context.top = 0;
  context.size = 0;

  value->type = Type::Null;

  ParseWhitespace(&context);

  Result result = ParseValue(&context, value);

  if (result == Result::OK) {
    ParseWhitespace(&context);

    if (*context.json != '\0') {
      return Result::RootNotSingular;
    }
  }

  assert(context.top == 0);
  delete context.stack;

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
    case '\"':
      return ParseString(context, value);
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

Result JSON::ParseString(Context* context, Value* value) {
  EXPECT(context, '\"');

  std::size_t top = context->top;
  std::size_t length = 0;
  const char* p = context->json;

  for (;;) {
    // get a character and move p forward by 1 step
    char character = *p++;

    switch (character) {
      case '\"':
        length = context->top - top;
        SetString(value, reinterpret_cast<char*>(ContextPop(context, length)),
                  length);
        context->json = p;
        return Result::OK;
      case '\0':
        context->top = top;
        return Result::MissingQuotationMark;
      case '\\':  // escape
        switch (*p++) {
          case '\"':
            PUTCHAR(context, '\"');
            break;
          case '\\':
            PUTCHAR(context, '\\');
            break;
          case '/':
            PUTCHAR(context, '/');
            break;
          case 'b':
            PUTCHAR(context, '\b');
            break;
          case 'f':
            PUTCHAR(context, '\f');
            break;
          case 'n':
            PUTCHAR(context, '\n');
            break;
          case 'r':
            PUTCHAR(context, '\r');
            break;
          case 't':
            PUTCHAR(context, '\t');
            break;
          default:
            context->top = top;
            return Result::InvalidStringEscape;
        }
      default:
        if (static_cast<unsigned char>(character) < 0x20) {
          context->top = top;
          return Result::InvalidStringCharacter;
        }
        PUTCHAR(context, character);
    }
  }
}

void* JSON::ContextPush(Context* context, size_t size) {
  assert(size > 0);

  if (context->top + size > context->size) {
    if (0 == context->size) {
      context->size = JPP_STACK_INIT_SIZE;
    }

    while (context->top + size >= context->size) {
      // context size * 1.5
      context->size += context->size >> 1;
    }

    // re-allocate memory for stack
    context->stack =
        reinterpret_cast<char*>(realloc(context->stack, context->size));
  }
  // return new start
  void* ret = context->stack + context->top;
  // move top to new end
  context->top += size;

  return ret;
}

void* JSON::ContextPop(Context* context, size_t size) {
  assert(context->top >= size);

  // move top back with (size * byte) and return new end
  return context->stack + (context->top -= size);
}

void JSON::InitValue(Value* value) {
  assert(value != nullptr);

  value->type = Type::Null;
}

void JSON::FreeValue(Value* value) {
  assert(value != nullptr);

  // clear string in value
  if (value->type == Type::String) {
    delete value->string.literal;
  }

  // clear value type
  value->type = Type::Null;
}

Type JSON::GetType(const Value* value) { return value->type; }

void JSON::SetNull(Value* value) { JSON::FreeValue(value); }

bool JSON::GetBoolean(const Value* value) {
  assert(value != nullptr &&
         (value->type == Type::False || value->type == Type::True));

  return value->boolean;
}

void JSON::SetBoolean(Value* value, bool boolean) {
  assert(value != nullptr);

  value->boolean = boolean;
  value->type = boolean ? Type::True : Type::False;
}

double JSON::GetNumber(const Value* value) {
  assert(value != nullptr && value->type == Type::Number);

  return value->number;
}

void JSON::SetNumber(Value* value, double number) {
  assert(value != nullptr);

  value->number = number;
  value->type = Type::Number;
}

const char* JSON::GetString(const Value* value) {
  assert(value != nullptr && value->type == Type::String);

  return value->string.literal;
}

std::size_t JSON::GetStringLength(const Value* value) {
  assert(value != nullptr && value->type == Type::String);

  return value->string.length;
}

void JSON::SetString(Value* value, const char* str, std::size_t length) {
  // string length can be zero
  assert(value != nullptr && (str != nullptr || length == 0));

  // clear value first
  SetNull(value);
  // allocate memory for string literal
  value->string.literal = reinterpret_cast<char*>(malloc(length + 1));
  std::memcpy(value->string.literal, str, length);
  // ends with '\0'
  value->string.literal[length] = '\0';
  // set string length
  value->string.length = length;
  // set value type as String
  value->type = Type::String;
}

}  // namespace jpp
