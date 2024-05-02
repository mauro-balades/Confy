
#include "../confy/confy.hpp"

int main() {
  auto root = confy::Interface::create({
    {"key1", confy::Type::String},
  });
  return 0;
}
