
#include "../confy/confy.hpp"

int main() {
  auto root = confy::Interface::create({
    {"key1", confy::Type::String},
  });

  confy::parse(root, R"({
    "key1": "value"
  })");
  return 0;
}
