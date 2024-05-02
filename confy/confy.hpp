#if 0 // Do not delete this block! License Check!
/**
 * MIT License
 * 
 * Copyright (c) 2024 mauro 🤙
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
/**
 * Source Code At: https://github.com/mauro-balades/Confy 
 * Have fun! 🤙
 */
/**
 * Notices: We require the c++ standard 17 or higher. 
 *   We make use of smart pointers and other modern c++ features.
 * You can define the following macros to add some custom behavior:
 *   - CONFY_NO_EXCEPTIONS: If you define this macro, we will not use exceptions.
 *   - CONFY_NO_NAMESPACE: If you define this macro, we will declare the namespace confy. (not recommended)
 *   - CONFY_JUST_DECLARATIONS: If you define this macro, we will not define the implementation of the functions.
 *   - CONFY_NO_ASSERT: If you define this macro, we will not use the assert macro.
 */

#endif // Finish License Check!

#ifndef __CONFY_MAIN_H__
#define __CONFY_MAIN_H__

#include <string>
#include <vector>
#include <memory>

#ifndef CONFY_NO_EXCEPTIONS
#include <stdexcept>
#endif

#ifndef CONFY_NO_ASSERT
#include <cassert>
#define CONFY_ASSERT(x) assert(x)
#else
#define CONFY_ASSERT(x)
#endif

namespace confy {

class Interface;
class Type;

/**
 * @brief A type class to represent the type of a key in the configuration.
 *
 * This is to make sure that the configuration is correct and that the parser
 * will not throw an exception if the key satisfies the schema.
 */
class Type {
public:
  Type();
  Type(const Type& other);
  Type(Type&& other);

  virtual ~Type() = default;
  virtual std::string name() const = 0;
  virtual bool is(const Type* other) const = 0;
  bool is(const std::shared_ptr<Type>& other) const;

  static std::shared_ptr<Type> String;
  static std::shared_ptr<Type> Number;
  static std::shared_ptr<Type> Object(std::vector<std::pair<std::string, std::shared_ptr<Type>>>& types);
  static std::shared_ptr<Type> Array(std::shared_ptr<Type>& type);
};

class StringType final : public Type {
public:
  virtual std::string name() const override;
  virtual bool is(const Type* other) const override;

  static std::shared_ptr<Type> create();
};

class NumType final : public Type {
public:
  virtual std::string name() const override;
  virtual bool is(const Type* other) const override;

  static std::shared_ptr<Type> create();
};

class ObjectType final : public Type {
public:
  using TypePair = std::pair<std::string, std::shared_ptr<Type>>;

  ObjectType(std::vector<TypePair> types);
  virtual std::string name() const override;
  virtual bool is(const Type* other) const override;
  
  std::shared_ptr<Type> get(const std::string& key) const;
  std::vector<TypePair> get_types() const;
  bool has(const std::string& key) const;

  virtual ~ObjectType() = default;

  static std::shared_ptr<Type> create(std::vector<TypePair>& types);
private:
  std::vector<TypePair> types;
};

class ArrayType final : public Type {
public:
  ArrayType(std::shared_ptr<Type> type);
  virtual std::string name() const override;
  virtual bool is(const Type* other) const override;

  std::shared_ptr<Type> get() const;

  virtual ~ArrayType() = default;

  static std::shared_ptr<Type> create(std::shared_ptr<Type>& type);
private:
  std::shared_ptr<Type> type;
};

/**
 * @brief The main class to manage the configuration root interface.
 *
 * This class is the main class to manage the configuration root interface.
 * Root interfaces are used to create a schema for the configuration.
 * If the parser finds a key that is not in the schema, it will throw an exception.
 *
 * Example:
 * @code
 * // Create a root interface
 * auto root = confy::RootInterface::create({
 *     {"key1", confy::Type::String},
 * });
 */
class Interface {
public:
  using Global = std::pair<std::string, std::shared_ptr<Type>>;

  Interface(std::vector<Global>& types);
  virtual ~Interface() = default;

  std::shared_ptr<Type> get(const std::string& key) const;
  std::vector<Global> get_globals() const;
  bool has(const std::string& key) const;

  static Interface create(std::vector<Global> types);
private:
  std::vector<Global> global;
};

namespace utils {
template<typename T, typename U>
[[nodiscard]] std::shared_ptr<T> as(std::shared_ptr<U>& type) {
  return std::dynamic_pointer_cast<T>(type);
}

template<typename T, typename U>
[[nodiscard]] T* as(U* type) {
  return dynamic_cast<T*>(type);
}

template<typename T, typename U>
[[nodiscard]] bool is(const std::shared_ptr<U>& type) {
  return as<T>(type) != nullptr;
}

template<typename T, typename U>
[[nodiscard]] bool is(const U* type) {
  return as<T>(type) != nullptr;
}

} // namespace utils

} // namespace confy

#ifdef CONFY_NO_NAMESPACE
using namespace confy;
#endif

#ifndef CONFY_JUST_DECLARATIONS
#include "confy.ipp"
#endif

#endif // __CONFY_MAIN_H__
