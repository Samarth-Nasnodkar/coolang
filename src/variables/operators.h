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
  {KEY_ADD, '+', true},
  {KEY_SUB, '-', true},
  {KEY_MUL, '*', true},
  {KEY_DIV, '/', true},
  {KEY_MOD, '%', true},
  {KEY_POW, '^', true},
  {KEY_LPAREN, '(', false},
  {KEY_RPAREN, ')', false},
  {KEY_LBRACE, '{', false},
  {KEY_RBRACE, '}', false},
  {KEY_LBRACKET, '[', false},
  {KEY_RBRACKET, ']', false},
};

#endif // OPERATORS_H