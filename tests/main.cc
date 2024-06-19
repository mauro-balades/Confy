
#define CONFY_USE_UTILS
#include "../src/confy.hpp"

#include <iostream>

int main() {
  auto root = confy::Interface::create({
    {"project", confy::Type::Object({
        {"name", confy::Type::String},
        {"version", confy::Type::String},
        {"author", confy::Type::Array(confy::Type::String)}, 
        {"description", confy::Type::String},
        {"log_level", confy::RangeNumType<0, 3>::create()}
    })},
    {"hello", confy::Type::String}
  });

  auto result = confy::parse_file(root);
  if (result.has_errors()) {
    for (const auto& error : result.get_errors()) {
      std::cout << "err: " << error.get_message() << std::endl;
      std::cout << "line: " << error.get_position().line << std::endl;
      std::cout << "column: " << error.get_position().column << std::endl;
    }
    return 1;
  }

  auto key1 = result.get_number_or("project.log_level", 0);
  std::cout << "key1: " << key1 << std::endl;

  return 0;
}
