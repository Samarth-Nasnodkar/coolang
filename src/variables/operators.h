#include "../includes.h"

#ifndef OPERATORS_H
#define OPERATORS_H

struct Operator {
  std::string name;
  char op;
  bool binary;
};

const Operator operators[] = {
  (Operator){"ADD", '+', true},
  (Operator){"SUB", '-', true},
  (Operator){"MUL", '*', true},
  (Operator){"DIV", '/', true},
  (Operator){"MOD", '%', true},
  (Operator){"POW", '^', true},
  (Operator){"LPAREN", '(', false},
  (Operator){"RPAREN", ')', false},
};

#endif // OPERATORS_H