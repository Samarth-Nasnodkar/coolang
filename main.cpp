#include <iostream>
#include "src/coolang.h"

int main(int argc, char **argv) {
  Coolang().run(argc > 1 ? argv[1] : "demo.coo");
  return 0;
}