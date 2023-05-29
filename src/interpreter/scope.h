#include "../includes.h"
#include "../variables/data.h"

#ifndef SCOPE_H
#define SCOPE_H

struct obj {
  std::string _name;
  std::string _type;
  std::map<std::string, data *> _data;
};

struct scope {
  int index;
  std::map<std::string, data *> localScope;
  std::map<std::string, node *> functionScope;
  std::map<std::string, node *> objectDefinition;
  std::map<std::string, obj *> objectScope;
  bool function = false;
};

#endif