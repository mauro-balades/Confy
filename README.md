
<img src="assets/logo.png" height="150" align="left"/>

**Confy**
Configure projects effortlessly! 💻

This a project configuration file parser and system originally created for the [Snowball Programming Language](https://github.com/snowball-lang)!

[Issues](https://github.com/mauro-balades/confy/issues)

## Features

* Header only C++ library (bringing fast parsing times)
* Statically types (with syntax previously defined by the project executor)
* Enables efficient memory management through smart pointers
* Cross-platform compatibility for versatile deployment
* Comprehensive error handling for robust development

## Show Me The Syntax!

```c++
  auto root = confy::Interface::create({
    {"project", confy::Type::Object({
        {"name", confy::Type::String},
        {"version", confy::Type::String},
        {"authors", confy::Type::Array(confy::Type::String)},
        {"description", confy::Type::String},
    })},
  });

  auto result = confy::parse_file(root, "./project.confy");
  // { errors: {}, root: ... }
```

Will be able to parse:

```conf
# haha, you cant see me!

project {
    name = "MyLib"
    version = "1.0.0"
    author = ["Your Name"]
    description = "A project created with Confy!"
}
```

# Data Types

### Object

* A set of values of values that can be accessed by name

```c++
Types::Object({
  {"name": Type},
  ...
});
```

```js
  myObject {
    name = type
  }
```

### Array

* A set of values with the same type that can contain an infinite ammount of elements (from 0 to infinity)

```c++
Types::Array(Type);
```

```js
  myArray = ["Hello", "Adios", "Ñog"]
```

### String

* An array of characters (it's just a string) 

```c++
Types::String;
```

```js
  myName = "mauro!"
```

### Number

* A number from -inf to inf that can contain decimal places (represented as a double in the backend)

```c++
Types::Number;
```

```js
  myNumber = 25
```

## Utility Types

* Utility types are used to validate the given data.
* To enable the utility classes, define the `CONFY_USE_UTILS` macro before including `confy.hpp`
* If the validation returns an error, it will be thrown and the parsing will fail

### MinNumType<int N1>

* A number that must be bigger than `N1`

```c++
MinNumType<10>::create();
```

```js
  myNumber = 12
```


### MaxNumType<int N1>

* A number that must be less than `N1`

```c++
MaxNumType<10>::create();
```

```js
  myNumber 6
```

### RangeNumType<int N1, int N2>

* A number that must be bigger than `N1`and less than `N2`

```c++
RangeNumType<10, 20>::create();
```

```js
  myNumber = 12
```

### MinStrType<int S1>

* The parsed string's length must be bigger than `S1`

```c++
MinStrType<3>::create();
```

```js
  myString = "hello there"
```

### StrRegexType

* The parsed string's must satisfy the given regex

```c++
StrRegexType::create("*");
```

```js
  myString = "I dont know how regex works"
```

### Custom Validation Types

* To create your own validation type, create a new class inheriting from a `primitive type` (like `NumType` or `StringType`).
* Override the `validate` method where it's value may depend from which type is being inherited from.

Example of a class that only accepts if a string starts with "hello".

```c++
class MyCustomType : public confy::StringType {
public:
  // Validate takes a `double` if it inherits from `confy::NumType`!
  std::optional<std::string> validate(const std::string& value) const override {
    if (s.rfind("hello", 0) == 0) { 
      return std::nullopt;
    }
    return "String must start with 'hello'!";
  }

  // Optional but it's nice to have a generator function
  static std::shared_ptr<MyCustomType> create() {
    return std::make_shared<MyCustomType>();
  }
}
```

Usage

```c++
  {"myStr": MyCustomType::create()}
```

This will work as (ignore the duplicate name error):

```py
  myStr = "hello world"

  myStr = "hello mauro"

  # error!
  myStr = "goodbye :("
```
