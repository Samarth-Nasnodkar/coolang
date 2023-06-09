#ifndef NODE_TYPE_H
#define NODE_TYPE_H

enum node_type {
  _number,
  _boolean,
  _str,
  _list,
  _index,
  _unary_operator,
  _binary_operator,
  _variable_assign,
  _variable_reassign,
  _variable_access,
  _code_block,
  _function_block,
  _if_else,
  _while_loop,
  _function_definition,
  _function_call,
  _method_definition,
  _method_call,
  _object_definition,
  _object_body,
  _dot,
  _return,
  _field_assign,
  _field_access,
  _nullnode
};

#endif // NODE_TYPE_H