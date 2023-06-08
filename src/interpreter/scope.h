#include "../includes.h"
#include "../variables/data.h"
#include "../parser/node.h"
#include "runtime_type.h"

#ifndef SCOPE_H
#define SCOPE_H

struct obj {
  std::string _name;
  std::string _type;
  std::map<std::string, __runtime_var> _data;
  node* _def;
};

struct scope {
  int index;
  std::map<std::string, __runtime_var> localScope;
  std::map<std::string, node *> objectDefinition;
  bool function = false;
};

#endif