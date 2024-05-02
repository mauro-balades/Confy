
<img src="assets/logo.png" height="150" align="left"/>

**Confy**
Configure projects effortlessly! 💻

This a project configuration file parser and system originally created for the [Snowball Programming Language](https://github.com/snowball-lang)!<br />

## Features

* Header only C++ library (bringing fast parsing times)
* Statically types (with syntax previously defined by the project executor)
* Enables efficient memory management through smart pointers
* Extensive documentation for quick and easy usage
* Cross-platform compatibility for versatile deployment
* Seamless interoperability with other programming languages
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
    name: "MyLib"
    version: "1.0.0"
    author: "Your Name"
    description: "A project created with Confy!"
}
```
