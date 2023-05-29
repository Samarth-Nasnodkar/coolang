#include "node_type.h"
#include "../lexer/token.h"

#ifndef NODE_H
#define NODE_H

struct node {
  Token token;
  node_type value;
  node *left;
  node *right;
  std::vector<node *> children;
  std::vector<Token> args;
  bool isConst;
};

std::string node_type_to_string(node_type type) {
  switch (type) {
    case node_type::_number:
      return "number";
    case node_type::_str:
      return "str";
    case node_type::_unary_operator:
      return "unary_operator";
    case node_type::_binary_operator:
      return "binary_operator";
    case node_type::_variable_assign:
      return "variable_assign";
    case node_type::_variable_reassign:
      return "variable_reassign";
    case node_type::_variable_access:
      return "variable_access";
    case node_type::_code_block:
      return "code_block";
    case node_type::_function_block:
      return "function_block";
    case node_type::_if_else:
      return "if_else";
    case node_type::_while_loop:
      return "while_loop";
    case node_type::_function_definition:
      return "function_definition";
    case node_type::_function_call:
      return "function_call";
    case node_type::_return:
      return "return";
    case node_type::_nullnode:
      return "nullnode";
    case node_type::_boolean:
      return "boolean";
    case node_type::_list:
      return "list";
    case node_type::_object_definition:
      return "object_definition";
    case node_type::_object_body:
      return "object_body";
    case node_type::_method_call:
      return "method_call";
    case node_type::_method_definition:
      return "method_definition";
    case node_type::_field_assign:
      return "field_assign";
    case node_type::_field_access:
      return "field_access";
    case node_type::_dot:
      return "dot";
    default:
      return "unknown";
  }
}

void printNode(node *node) {
  if (node == nullptr) return;
  std::cout << node->token.to_string() << "(" << node_type_to_string(node->value) << ") ";
  std::cout << "{ ";
  if (node->left) {
    printNode(node->left);
  }
  if (node->right) {
    std::cout << ", ";
    printNode(node->right);
  }
  if (!node->children.empty()) {
    std::cout << ", ";
    for(auto child : node->children) {
      printNode(child);
    }
  }
  std::cout << "} ";
}

#endif // NODE_H