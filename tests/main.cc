
#include "../confy/confy.hpp"

int main() {
  auto root = confy::RootInterface::create({
    {"key1", confy::Type::String},
  });
  return 0;
}
