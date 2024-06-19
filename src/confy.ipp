
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

std::shared_ptr<Type> Type::Object(std::vector<std::pair<std::string, std::shared_ptr<Type>>> types) {
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

std::optional<std::string> StringType::validate(const std::string& value) const {
  return std::nullopt;
}

std::string NumType::name() const {
  return "number";
}

bool NumType::is(const Type* other) const {
  return utils::is<const NumType>(other);
}

std::optional<std::string> NumType::validate(double value) const {
  return std::nullopt;
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

bool ObjectType::has(const std::string& key) const {
  for (const auto& type : types) {
    if (type.first == key) {
      return true;
    }
  }
  return false;
}

std::shared_ptr<Type> ObjectType::get(const std::string& key) const {
  for (const auto& type : types) {
    if (type.first == key) {
      return type.second;
    }
  }
  return nullptr;
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

std::shared_ptr<Type> ArrayType::get() const {
  return type;
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

Interface::Interface(std::vector<Interface::Global>& globals) {
  std::vector<ObjectType::TypePair> global_types;
  for (const auto& global : globals) {
    global_types.push_back({global.first, global.second});
  }
  global = std::make_shared<ObjectType>(global_types);
}

std::shared_ptr<ObjectType> Interface::get_globals() const {
  return global;
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

std::optional<std::shared_ptr<Value>> Object::get(const std::string& key) const {
  if (values.find(key) != values.end()) {
    return values.at(key);
  }
  return std::nullopt;
}

bool Object::has(const std::string& key) const {
  return values.find(key) != values.end();
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
  return as_number();
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

Result::Result(Result::RootType values, std::string config, std::vector<Error> errors)
  : root(values), config(config), errors(errors) {}

Result::RootType Result::get_root() const {
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

Result Result::create(Result::RootType values, std::string config, std::vector<Error> errors) {
  return Result(values, config, errors);
}

std::optional<std::shared_ptr<Value>> Result::search(const std::string& key) const {
  auto parts = utils::split(key, ".");
  std::shared_ptr<Value> current = nullptr;
  for (const auto& part : parts) {
    if (!current) {
      if (root.find(part) == root.end()) {
        return std::nullopt;
      }
      current = root.at(part);
    } else {
      if (!current->is_object()) {
        return std::nullopt;
      }
      auto obj = utils::as<Object>(current);
      if (!obj->has(part)) {
        return std::nullopt;
      }
      current = obj->get(part).value();
    }
  }
  return current;
}

std::optional<double> Result::get_number(const std::string& key) const {
  if (auto val = search(key)) {
    if (val.value()->is_number()) {
      return utils::as<Number>(val.value())->get_value();
    }
  }
  return std::nullopt;
}

std::optional<std::string> Result::get_string(const std::string& key) const {
  if (auto val = search(key)) {
    if (val.value()->is_string()) {
      return utils::as<String>(val.value())->get_value();
    }
  }
  return std::nullopt;
}

std::optional<std::unordered_map<std::string, std::shared_ptr<Value>>> Result::get_object(const std::string& key) const {
  if (auto val = search(key)) {
    if (val.value()->is_object()) {
      return utils::as<Object>(val.value())->get_values();
    }
  }
  return std::nullopt;
}

std::optional<std::vector<std::shared_ptr<Value>>> Result::get_array(const std::string& key) const {
  if (auto val = search(key)) {
    if (val.value()->is_array()) {
      return utils::as<Array>(val.value())->get_values();
    }
  }
  return std::nullopt;
}

double Result::get_number_or(const std::string& key, double def) const {
  if (auto val = get_number(key)) {
    return val.value();
  }
  return def;
}

std::string Result::get_string_or(const std::string& key, std::string def) const {
  if (auto val = get_string(key)) {
    return val.value();
  }
  return def;
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

std::vector<std::string> utils::split(std::string str, const std::string& delim) {
  std::vector<std::string> tokens;
  size_t pos = 0;
  while ((pos = str.find(delim)) != std::string::npos) {
    tokens.push_back(str.substr(0, pos));
    str.erase(0, pos + delim.length());
  }
  tokens.push_back(str);
  return tokens;
}

namespace parser_internal {

#define PARSER_NEXT_CHAR() \
  SKIP_WHITESPACE(false); \
  if (!(char_index >= config.size())) { \
    pos.column++; \
    char_index++; \
  } \

#define SKIP_WHITESPACE(and_newline) \
  while (config[char_index] == ' ' || config[char_index] == '\t' || (and_newline && config[char_index] == '\n')) { \
    pos.column++; \
    if (config[char_index] == '\n') { \
      pos.line++; \
      pos.column = 1; \
    } \
    char_index++; \
  } \
  if (config[char_index] == '#') { \
    while (config[char_index] != '\n') { \
      char_index++; \
    } \
    pos.line++; \
    pos.column = 1; \
  } 

#define PARSER_EXPECT_CHAR(c) \
  SKIP_WHITESPACE(false); \
  if (config[char_index] != c) { \
    return create_error("Expected '" + std::string(1, c) + "' and got '" + std::string(1, config[char_index]) + "'", pos, errors); \
  } \
  PARSER_NEXT_CHAR();

#define EXPECT_END_OF_VALUE() \
  if (!as_value) { \
    SKIP_WHITESPACE(false); \
    if (config[char_index] != '\n') { \
      create_error("Expected '<newline>' and got '" + std::string(1, config[char_index]) + "'", pos, errors); \
      return std::nullopt; \
    } \
    pos.line++; \
    pos.column = 1; \
    char_index++; \
  }

std::optional<std::string> get_identifier(const std::string& config, size_t& char_index, Error::Position& pos) {
  SKIP_WHITESPACE(true);
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

bool create_error(const std::string& message, Error::Position& pos, std::vector<Error>& errors) {
  errors.push_back(Error(message, pos));
  return EXIT_FAILURE;
}

std::optional<std::shared_ptr<Value>> parse_value(std::shared_ptr<ObjectType> root, const std::string& config, 
  Result::RootType& values, 
  std::vector<Error>& errors, Error::Position& pos, 
  size_t& char_index, const std::optional<std::string>& identifier, bool as_value = false, bool use_equals = true) {
  SKIP_WHITESPACE(false);
  auto val_type = root->get(*identifier);
  if (nullptr == val_type) {
    create_error("Unknown identifier '" + *identifier + "'", pos, errors);
    return std::nullopt;
  }
  if (use_equals) {
    if (config[char_index] != '=') {
      create_error("Expected '=' and got '" + std::string(1, config[char_index]) + "'", pos, errors);
      return std::nullopt;
    }
    PARSER_NEXT_CHAR();
  }
  SKIP_WHITESPACE(false);
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
    if (!val_type->is<StringType>()) {
      create_error("Expected '" + val_type->name() + "' and got '" + value + "'", pos, errors);
      return std::nullopt;
    }
    EXPECT_END_OF_VALUE();
    if (auto err = utils::as<StringType>(val_type)->validate(value)) {
      create_error(err.value(), pos, errors);
      return std::nullopt;
    }
    return String::create(val_type, value);
  } else if (std::isdigit(config[char_index])) {
    std::string value;
    while (std::isdigit(config[char_index]) || config[char_index] == '.') {
      value += config[char_index];
      pos.column++;
      char_index++;
    }
    if (!val_type->is<NumType>()) {
      create_error("Expected '" + val_type->name() + "' and got 'number'", pos, errors);
      return std::nullopt;
    }
    EXPECT_END_OF_VALUE();
    auto num = std::stod(value);
    if (auto err = utils::as<NumType>(val_type)->validate(num)) {
      create_error(err.value(), pos, errors);
      return std::nullopt;
    }
    return Number::create(val_type, num);
  } else if (config[char_index] == '{') {
    PARSER_NEXT_CHAR();
    if (config[char_index] != '\n') {
      create_error("Expected '<newline>' and got '" + std::string(1, config[char_index]) + "'", pos, errors);
      return std::nullopt;
    }
    SKIP_WHITESPACE(false)
    std::unordered_map<std::string, std::shared_ptr<Value>> rvalues;
    if (!val_type->is<ObjectType>()) {
      create_error("Expected '" + val_type->name() + "' and got 'object'", pos, errors);
      return std::nullopt;
    }
    while (config[char_index] != '}') {
      SKIP_WHITESPACE(false);
      if (config[char_index] == '}') {
        break;
      }
      auto identifier = get_identifier(config, char_index, pos);
      if (!identifier) {
        create_error("Expected identifier and got '" + std::string(1, config[char_index]) + "'", pos, errors);
        return std::nullopt;
      }
      SKIP_WHITESPACE(false);
      // TODO: set the new root here!
      auto val = parse_value(utils::as<ObjectType>(val_type), config, values, errors, pos, char_index, identifier, false);
      if (!val) {
        return std::nullopt;
      }
      if (rvalues.find(*identifier) != rvalues.end()) {
        create_error("Duplicate identifier '" + *identifier + "'", pos, errors);
        return std::nullopt;
      }
      rvalues[*identifier] = *val;
    }
    PARSER_NEXT_CHAR();
    EXPECT_END_OF_VALUE();
    return Object::create(val_type, rvalues);
  } else if (config[char_index] == '[') {
    PARSER_NEXT_CHAR();
    std::vector<std::shared_ptr<Value>> rvalues;
    if (!val_type->is<ArrayType>()) { // TODO: Check for array subtypes
      create_error("Expected '" + val_type->name() + "' and got 'array'", pos, errors);
      return std::nullopt;
    }
    auto as_array = utils::as<ArrayType>(val_type);
    while (config[char_index] != ']') {
      auto temp_root = std::make_shared<ObjectType>(std::vector<ObjectType::TypePair> {
        {"$temp", as_array->get()}
      });
      auto val = parse_value(temp_root, config, values, errors, pos, char_index, "$temp", true, false);
      if (!val) {
        return std::nullopt;
      }
      rvalues.push_back(*val);
      if (config[char_index] == ',') {
        PARSER_NEXT_CHAR();
        continue;
      }
      if (config[char_index] == ']') {
        break;
      } else {
        create_error("Expected ',' or ']' and got '" + std::string(1, config[char_index]) + "'", pos, errors);
        return std::nullopt;
      }
    }
    PARSER_NEXT_CHAR();
    EXPECT_END_OF_VALUE();
    return Array::create(val_type, rvalues);
  }
  create_error("Expected a valid value but found '" + std::string(1, config[char_index]) + "'", pos, errors);
  return std::nullopt;
}

bool parse_global_rule(std::shared_ptr<ObjectType> root, const std::string& config, 
  Result::RootType& values, 
  std::vector<Error>& errors, Error::Position& pos, 
  size_t& char_index, bool is_global = false) {
  Error::Position copy_pos = pos;
  auto identifier = get_identifier(config, char_index, pos);
  if (!identifier) {
    return create_error("Expected identifier and got '" + std::string(1, config[char_index]) + "'", pos, errors);
  }

  if (!root->has(*identifier)) {
    return create_error("Unknown identifier '" + *identifier + "'", copy_pos, errors);
  }

  SKIP_WHITESPACE(false);
  if (is_global && config[char_index] != '{') {
    auto val = parse_value(root, config, values, errors, pos, char_index, identifier, false);
    if (!val) {
      return EXIT_FAILURE;
    }
    values.insert(std::make_pair(*identifier, *val));
    return EXIT_SUCCESS;
  }

  if (!is_global) {
  }

  PARSER_EXPECT_CHAR('{');
  char_index--; // We need to reparse the '{' character

  // It's the same syntax as the global rule
  auto val = parse_value(root, config, values, errors, pos, char_index, identifier, false, false);
  if (!val) {
    return EXIT_FAILURE;
  }
  values.insert(std::make_pair(*identifier, *val));
  PARSER_NEXT_CHAR();
  return EXIT_SUCCESS;
}

bool parse_global(Interface& root, const std::string& config, 
  Result::RootType& values, 
  std::vector<Error>& errors, Error::Position& pos, 
  size_t& char_index) {

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
        return parse_global_rule(root.get_globals(), config, values, errors, pos, char_index, true);
      }
    }
  }
  return EXIT_FAILURE; // End the loop
}

} // namespace _internal

Result parse(Interface& root, const std::string& config) {
  std::vector<Error> errors;
  Result::RootType values;
  Error::Position pos = {1, 1};
  size_t char_index = 0;
  while (true) {
    if (parser_internal::parse_global(root, config, values, errors, pos, char_index)) {
      break;
    }
  }
  return Result::create(values, config, errors);
}

Result parse_file(Interface& root, const std::string& file) {
  std::ifstream ifs(file);
  if (!ifs.is_open()) {
    return Result::create({}, "", {Error("Could not open file", Error::Position {1, 1})});
  }
  std::string config((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
  return parse(root, config);
}

#undef PARSER_NEXT_CHAR
#undef PARSER_EXPECT_CHAR

} // namespace confy
