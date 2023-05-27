#ifndef NODE_TYPE_H
#define NODE_TYPE_H

enum node_type {
  _number,
  _boolean,
  _str,
  _unary_operator,
  _binary_operator,
  _variable_assign,
  _variable_reassign,
  _variable_access,
  _code_block,
  _function_block,
  _if_else,
  _while_loop,
  _return,
  _nullnode
};

#endif // NODE_TYPE_H