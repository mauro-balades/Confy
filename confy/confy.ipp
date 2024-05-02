
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

} // namespace confy
