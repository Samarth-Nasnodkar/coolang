#include <iostream>
#include "src/coolang.h"

int main(int argc, char **argv) {
  Coolang coolang;
  coolang.run(argv[1]);
  return 0;
}