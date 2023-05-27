#include "../includes.h"
#include "keywords.h"

#ifndef SHORT_HAND_OP
#define SHORT_HAND_OP

struct short_hand_op {
  std::string name;
  std::string op;
  std::string binary_op_name;
};

const short_hand_op short_hand_ops[] = {
  {KEY_PLUS_EQUALS, "+=", KEY_ADD},
  {KEY_MINUS_EQUALS, "-=", KEY_SUB},
  {KEY_MUL_EQUALS, "*=", KEY_MUL},
  {KEY_DIV_EQUALS, "/=", KEY_DIV},
  {KEY_MOD_EQUALS, "%=", KEY_MOD},
  {KEY_POW_EQUALS, "^=", KEY_POW},
};

#endif