#ifndef NODE_TYPE_H
#define NODE_TYPE_H

enum node_type {
  _number,
  _boolean,
  _unary_operator,
  _binary_operator,
  _variable_assign,
  _variable_access,
  _code_block,
  _nullnode
};

#endif // NODE_TYPE_H