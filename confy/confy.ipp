
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

} // namespace confy
