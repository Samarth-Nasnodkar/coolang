#include "../includes.h"
#include "keywords.h"

#ifndef CMP_H
#define CMP_H

struct Cmp {
  std::string name;
  std::string op;
};

const Cmp cmps[] = {
  {KEY_LE, "<="},
  {KEY_GE, ">="},
  {KEY_LT, "<"},
  {KEY_GT, ">"},
  {KEY_EE, "=="},
  {KEY_NE, "!="},
};

#endif