#include "../includes.h"
#include "../variables/data.h"

#ifndef SCOPE_H
#define SCOPE_H

struct scope {
  int index;
  std::map<std::string, data> localScope;
};

#endif