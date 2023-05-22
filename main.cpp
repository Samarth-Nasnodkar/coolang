#include <iostream>
#include "src/coolang.h"

int main() {
  Coolang coolang;
  while (true) {
    std::string input;
    std::cout << "coo > ";
    std::getline(std::cin, input);
    if (input == ".exit") {
      break;
    }
    coolang.run(input);
  }
  return 0;
}