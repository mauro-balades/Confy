
#define CONFY_USE_UTILS
#include "../src/confy.hpp"

#include <iostream>

int main() {
  auto root = confy::Interface::create({
    {"project.log_level", confy::Type::Number},
  });

  auto result = confy::parse_file(root);
  if (result.has_errors()) {
    for (const auto& error : result.get_errors()) {
      std::cout << "err: " << error.get_message() << std::endl;
    }
    return 1;
  }

  auto key1 = result.get_number_or("project.log_level", 0);
  std::cout << "key1: " << key1 << std::endl;

  return 0;
}
