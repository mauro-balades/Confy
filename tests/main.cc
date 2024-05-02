
#include "../confy/confy.hpp"

#include <iostream>

int main() {
  auto root = confy::Interface::create({
    {"key1", confy::Type::String},
  });

  auto result = confy::parse(root, R"(hello: "world")");

  if (result.has_errors()) {
    for (const auto& error : result.get_errors()) {
      std::cout << error.get_message() << std::endl;
    }
  }

  return 0;
}
