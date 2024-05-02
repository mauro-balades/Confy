
/**
 * DO NOT INCLUDE THIS FILE DIRECTLY!
 *
 * This is the implementation file for the confy library.
 * If you want to use the library, include the confy.hpp file.
 *
 */

namespace confy {

Type::Type() {}
Type::Type(const Type& other) = default;
Type::Type(Type&& other) = default;

bool Type::is(const std::shared_ptr<Type>& other) const {
  return is(reinterpret_cast<const Type*>(other.get()));
}

std::shared_ptr<Type> Type::String = StringType::create();
std::shared_ptr<Type> Type::Number = NumType::create();

std::shared_ptr<Type> Type::Object(std::vector<std::pair<std::string, std::shared_ptr<Type>>>& types) {
  return ObjectType::create(types);
}

std::shared_ptr<Type> Type::Array(std::shared_ptr<Type>& type) {
  return ArrayType::create(type);
}

std::string StringType::name() const {
  return "string";
}

bool StringType::is(const Type* other) const {
  return utils::is<const StringType>(other);
}

std::shared_ptr<Type> StringType::create() {
  return std::make_shared<StringType>();
}

std::string NumType::name() const {
  return "number";
}

bool NumType::is(const Type* other) const {
  return utils::is<const NumType>(other);
}

std::shared_ptr<Type> NumType::create() {
  return std::make_shared<NumType>();
}

ObjectType::ObjectType(std::vector<TypePair> types) : types(types) {}

std::vector<ObjectType::TypePair> ObjectType::get_types() const {
  return types;
}

std::string ObjectType::name() const {
  return "object";
}

bool ObjectType::is(const Type* other) const {
  if (const auto obj = utils::as<const ObjectType>(other)) {
    if (types.size() != obj->types.size()) {
      return false;
    }

    for (size_t i = 0; i < types.size(); i++) {
      if (types[i].first != obj->types[i].first || !types[i].second->is(obj->types[i].second)) {
        return false;
      }
    }

    return true;
  }
  return false;
}

std::shared_ptr<Type> ObjectType::create(std::vector<TypePair>& types) {
  return std::make_shared<ObjectType>(types);
}

ArrayType::ArrayType(std::shared_ptr<Type> type) : type(type) {}

std::string ArrayType::name() const {
  return "array";
}

bool ArrayType::is(const Type* other) const {
  if (const auto arr = utils::as<const ArrayType>(other)) {
    return type->is(arr->type);
  }
  return false;
}

std::shared_ptr<Type> ArrayType::create(std::shared_ptr<Type>& type) {
  return std::make_shared<ArrayType>(type);
}

Interface::Interface(std::vector<Interface::Global>& globals) : global(globals) {}

std::vector<Interface::Global> Interface::get_globals() const {
  return global;
}

std::shared_ptr<Type> Interface::get(const std::string& key) const {
  for (const auto& global : global) {
    if (global.first == key) {
      return global.second;
    }
  }
  return nullptr;
}

bool Interface::has(const std::string& key) const {
  for (const auto& global : global) {
    if (global.first == key) {
      return true;
    }
  }
  return false;
}

Interface Interface::create(std::vector<Interface::Global> globals) {
  return Interface(globals);
}

Value::Value(std::shared_ptr<Type> type) : type(type) {}

std::shared_ptr<Type> Value::get_type() const {
  return type;
}

std::shared_ptr<Value> Value::create(std::shared_ptr<Type> type) {
  return std::make_shared<Value>(type);
}

bool Value::is_array() const { return false; }
bool Value::is_number() const { return false; }
bool Value::is_object() const { return false; }
bool Value::is_string() const { return false; }

std::string Value::as_string() const {
  CONFY_ASSERT(false, "Value is not a string");
  return "";
}

double Value::as_number() const {
  CONFY_ASSERT(false, "Value is not a number");
  return 0;
}

std::unordered_map<std::string, std::shared_ptr<Value>> Value::as_object() const {
  CONFY_ASSERT(false, "Value is not an object");
  return {};
}

std::vector<std::shared_ptr<Value>> Value::as_array() const {
  CONFY_ASSERT(false, "Value is not an array");
  return {};
}

Object::Object(std::shared_ptr<Type> type, std::unordered_map<std::string, std::shared_ptr<Value>> values)
  : Value(type), values(values) {
    CONFY_ASSERT(utils::is<const ObjectType>(type), "Type is not an object");
  }

bool Object::is_object() const {
  return true;
}

std::unordered_map<std::string, std::shared_ptr<Value>> Object::as_object() const {
  return values;
}

std::unordered_map<std::string, std::shared_ptr<Value>> Object::get_values() const {
  return values;
}

Array::Array(std::shared_ptr<Type> type, std::vector<std::shared_ptr<Value>> values)
  : Value(type), values(values) {
    CONFY_ASSERT(utils::is<const ArrayType>(type), "Type is not an array");
  }

bool Array::is_array() const {
  return true;
}

std::vector<std::shared_ptr<Value>> Array::as_array() const {
  return values;
}

std::vector<std::shared_ptr<Value>> Array::get_values() const {
  return values;
}

String::String(std::shared_ptr<Type> type, std::string value) : Value(type), value(value) {
  CONFY_ASSERT(utils::is<const StringType>(type), "Type is not a string");
}

std::string String::as_string() const {
  return get_value();
}

bool String::is_string() const {
  return true;
}

Number::Number(std::shared_ptr<Type> type, double value) : Value(type), value(value) {
  CONFY_ASSERT(utils::is<const NumType>(type), "Type is not a number");
}

bool Number::is_number() const {
  return true;
}

double Number::as_number() const {
  return value;
}

double Number::get_value() const {
  return get_value();
}

std::string String::get_value() const {
  return value;
}

std::shared_ptr<Object> Object::create(std::shared_ptr<Type> type, std::unordered_map<std::string, std::shared_ptr<Value>>& values) {
  return std::make_shared<Object>(type, values);
}

std::shared_ptr<Array> Array::create(std::shared_ptr<Type> type, std::vector<std::shared_ptr<Value>>& values) {
  return std::make_shared<Array>(type, values);
}

std::shared_ptr<String> String::create(std::shared_ptr<Type> type, std::string value) {
  return std::make_shared<String>(type, value);
}

std::shared_ptr<Number> Number::create(std::shared_ptr<Type> type, double value) {
  return std::make_shared<Number>(type, value);
}

Result::Result(std::vector<Result::RootType> values, std::string config, std::vector<Error> errors)
  : root(values), config(config), errors(errors) {}

std::vector<Result::RootType> Result::get_root() const {
  return root;
}

std::string Result::get_config() const {
  return config;
}

std::vector<Error> Result::get_errors() const {
  return errors;
}

bool Result::has_errors() const {
  return !errors.empty();
}

Result Result::create(std::vector<RootType> values, std::string config, std::vector<Error> errors) {
  return Result(values, config, errors);
}

Error::Position Error::Position::copy() const {
  return Error::Position {line, column};
}

Error::Error(std::string message, Error::Position pos) : message(message), position(pos) {}

std::string Error::get_message() const {
  return message;
}

Error::Position Error::get_position() const {
  return position;
}

const char* Error::what() const noexcept {
  return message.c_str();
}

namespace parser_internal {

std::optional<std::string> get_identifier(const std::string& config, int& char_index, Error::Position& pos) {
  if (!std::isalpha(config[char_index])) {
    return std::nullopt;
  }
  std::string identifier;
  while (std::isalnum(config[char_index]) || config[char_index] == '_') {
    identifier += config[char_index];
    pos.column++;
    char_index++;
  }
  return identifier;
}

#define PARSER_NEXT_CHAR() \
  SKIP_WHITESPACE(); \
  if (!(char_index >= config.size())) { \
    pos.column++; \
    char_index++; \
  } \

#define SKIP_WHITESPACE() \
  while (config[char_index] == ' ' || config[char_index] == '\t' || config[char_index] == '\n') { \
    if (config[char_index] == '\n') { \
      pos.line++; \
      pos.column = 1; \
    } else { \
      pos.column++; \
    } \
    char_index++; \
  }

#define PARSER_EXPECT_CHAR(c) \
  SKIP_WHITESPACE(); \
  if (config[char_index] != c) { \
    return create_error("Expected '" + std::string(1, c) + "' and got '" + std::string(1, config[char_index]) + "'", pos, errors); \
  } \
  PARSER_NEXT_CHAR();

bool create_error(const std::string& message, Error::Position& pos, std::vector<Error>& errors) {
  errors.push_back(Error(message, pos));
  return EXIT_FAILURE;
}

std::optional<std::shared_ptr<Value>> parse_value(Interface& root, const std::string& config, 
  std::vector<Result::RootType>& values, 
  std::vector<Error>& errors, Error::Position& pos, 
  int& char_index, const std::optional<std::string>& identifier) {
  SKIP_WHITESPACE();
  if (config[char_index] == '"') {
    PARSER_NEXT_CHAR();
    std::string value;
    while (config[char_index] != '"') {
      value += config[char_index];
      pos.column++;
      char_index++;
      if (char_index >= config.size()) {
        create_error("Unexpected end of file", pos, errors);
        return std::nullopt;
      }
    }
    PARSER_NEXT_CHAR();
    return String::create(root.get(*identifier), value);
  } else if (std::isdigit(config[char_index])) {
    std::string value;
    while (std::isdigit(config[char_index]) || config[char_index] == '.') {
      value += config[char_index];
      pos.column++;
      char_index++;
    }
    return Number::create(root.get(*identifier), std::stod(value));
  } else if (config[char_index] == '{') {
    PARSER_NEXT_CHAR();
    std::unordered_map<std::string, std::shared_ptr<Value>> rvalues;
    while (config[char_index] != '}') {
      auto identifier = get_identifier(config, char_index, pos);
      if (!identifier) {
        create_error("Expected identifier and got '" + std::string(1, config[char_index]) + "'", pos, errors);
        return std::nullopt;
      }
      SKIP_WHITESPACE();
      if (config[char_index] != ':') {
        create_error("Expected ':' and got '" + std::string(1, config[char_index]) + "'", pos, errors);
        return std::nullopt;
      }
      auto val = parse_value(root, config, values, errors, pos, char_index, identifier);
      if (!val) {
        return std::nullopt;
      }
      rvalues[*identifier] = *val;
    }
    PARSER_NEXT_CHAR();
    return Object::create(root.get(*identifier), rvalues);
  } else if (config[char_index] == '[') {
    PARSER_NEXT_CHAR();
    std::vector<std::shared_ptr<Value>> rvalues;
    while (config[char_index] != ']') {
      auto val = parse_value(root, config, values, errors, pos, char_index, identifier);
      if (!val) {
        return std::nullopt;
      }
      rvalues.push_back(*val);
    }
    PARSER_NEXT_CHAR();
    return Array::create(root.get(*identifier), rvalues);
  }
  return std::nullopt;
}

bool parse_global_rule(Interface& root, const std::string& config, 
  std::vector<Result::RootType>& values, 
  std::vector<Error>& errors, Error::Position& pos, 
  int& char_index, bool is_global = false) {
  auto identifier = get_identifier(config, char_index, pos);
  if (!identifier) {
    return create_error("Expected identifier and got '" + std::string(1, config[char_index]) + "'", pos, errors);
  }

  SKIP_WHITESPACE();
  if (config[char_index] == ':' && is_global) {
    PARSER_NEXT_CHAR();
    auto val = parse_value(root, config, values, errors, pos, char_index, identifier);
    if (!val) {
      return EXIT_FAILURE;
    }
    values.push_back(std::make_pair(*identifier, *val));
    return EXIT_SUCCESS;
  }

  if (!is_global) {
    PARSER_EXPECT_CHAR(':');
    PARSER_NEXT_CHAR();
  }

  PARSER_EXPECT_CHAR('{');

  auto type = root.get(*identifier);
  if (!type) {
    return create_error("Unknown identifier '" + *identifier + "'", pos, errors);
  }

  PARSER_EXPECT_CHAR('}');
  return EXIT_SUCCESS;
}

bool parse_global(Interface& root, const std::string& config, 
  std::vector<Result::RootType>& values, 
  std::vector<Error>& errors, Error::Position& pos, 
  int& char_index) {
  while (char_index < config.size()) {
    switch (config[char_index]) {
      case ' ':
      case '\t':
        char_index++;
        pos.column++;
        break;

      case '\n':
        pos.column = 1;
        pos.line++;
        char_index++;
        break;

      case 0:
        return EXIT_FAILURE;

      default: {
        return parse_global_rule(root, config, values, errors, pos, char_index, true);
      }
    }
  }
  CONFY_ASSERT(false, "Unexpected end of file");
  return EXIT_FAILURE;
}

} // namespace _internal

Result parse(Interface& root, const std::string& config) {
  std::vector<Error> errors;
  std::vector<Result::RootType> values;
  Error::Position pos = {1, 1};
  int char_index = 0;
  while (true) {
    if (parser_internal::parse_global(root, config, values, errors, pos, char_index)) {
      break;
    }
  }
  return Result::create(values, config, errors);
}

#undef PARSER_NEXT_CHAR
#undef PARSER_EXPECT_CHAR

} // namespace confy
