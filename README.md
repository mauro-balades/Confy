
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
        {"author", confy::Type::String},
        {"description", confy::Type::String},
    })},
  });

  auto result = confy::parse_file(root, "./project.confy");
  // { errors: {}, root: ... }
```

Will be able to parse:

```js
project {
    name: "MyLib";
    version: "1.0.0";
    author: "Your Name";
    description: "A project created with Confy!";
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
  myObject: {
    name: type
  };
```

### Array

* A set of values with the same type that can contain an infinite ammount of elements (from 0 to infinity)

```c++
Types::Array(Type);
```

```js
  myArray: ["Hello", "Adios", "Ñog"];
```

### String

* An array of characters (it's just a string) 

```c++
Types::String;
```

```js
  myName: "mauro!";
```

### Number

* A number from -inf to inf that can contain decimal places (represented as a double in the backend)

```c++
Types::Number;
```

```js
  myNumber: 25;
```