
#define CONFY_USE_UTILS
#include "../confy/confy.hpp"

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
  });

  auto result = confy::parse_file(root);
  if (result.has_errors()) {
    for (const auto& error : result.get_errors()) {
      std::cout << "err: " << error.get_message() << std::endl;
    }
    return 1;
  }

  auto key1 = result.get_root().at("project")->as_object().at("name");
  if (key1->is_string()) {
    std::cout << key1->as_string() << std::endl;
  }

  return 0;
}
