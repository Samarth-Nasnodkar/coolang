#include "../includes.h"
#include "keywords.h"

#ifndef OPERATORS_H
#define OPERATORS_H

struct Operator {
  std::string name;
  char op;
  bool binary;
};

const Operator operators[] = {
  (Operator){KEY_ADD, '+', true},
  (Operator){KEY_SUB, '-', true},
  (Operator){KEY_MUL, '*', true},
  (Operator){KEY_DIV, '/', true},
  (Operator){KEY_MOD, '%', true},
  (Operator){KEY_POW, '^', true},
  (Operator){KEY_EQ, '=', true},
  (Operator){KEY_LPAREN, '(', false},
  (Operator){KEY_RPAREN, ')', false},
  (Operator){KEY_LBRACE, '{', false},
  (Operator){KEY_RBRACE, '}', false},
};

#endif // OPERATORS_H