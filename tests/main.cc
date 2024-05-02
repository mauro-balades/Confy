
#include "../confy/confy.hpp"

#include <iostream>

int main() {
  auto root = confy::Interface::create({
    {"key1", confy::Type::String},
  });

  auto result = confy::parse(root, R"(key1: 23)");

  if (result.has_errors()) {
    for (const auto& error : result.get_errors()) {
      std::cout << error.get_message() << std::endl;
    }
    return 1;
  }

  auto key1 = result.get_root()["key1"];
  if (key1->is_string()) {
    std::cout << key1->as_string() << std::endl;
  }

  return 0;
}
