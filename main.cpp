#include <iostream>
#include "src/coolang.h"

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "Usage: " << argv[0] << " <file>" << std::endl;
    return 1;
  }
  Coolang().run(argv[1]);
  return 0;
}