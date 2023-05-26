#include "../includes.h"
#include "keywords.h"

#ifndef LOGIC_OPERATOR_H
#define LOGIC_OPERATOR_H

struct logic_operator {
  std::string name;
  std::string op;
};

const logic_operator logic_ops[] = {
  {KEY_AND, "and"},
  {KEY_OR, "or"},
  {KEY_NOT, "not"},
};

#endif