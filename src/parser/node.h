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
    default:
      return "unknown";
  }
}

void printNode(node *node) {
  if (node == nullptr) return;
  if (node->value == node_type::_number || node->value == node_type::_str || node->value == node_type::_boolean) {
    std::cout << node->token.get_value().to_string() << "(" << node_type_to_string(node->value) << ") ";
  }
  else if (node->value == node_type::_unary_operator) {
    std::cout << node->token.to_string() << "(" << node_type_to_string(node->value) << ") ";
    printNode(node->right);
  }
  else if (node->value == node_type::_binary_operator) {
    std::cout << "(";
    printNode(node->left);
    std::cout << " " << node->token.to_string() << "(" << node_type_to_string(node->value) << ") ";
    printNode(node->right);
    std::cout << ")";
  } 
  else if (node->value == node_type::_variable_assign) {
    std::cout << node->token.to_string() << "(" << node_type_to_string(node->value) << ") = ";
    printNode(node->left);
  }
  else if (node->value == node_type::_variable_access) {
    std::cout << node->token.to_string() << "(" << node_type_to_string(node->value) << ") ";
  }
  else if (node->value == node_type::_code_block) {
    std::cout << "code_block(" << node_type_to_string(node->value) << ") ";
    std::cout << "{ ";
    for(auto child : node->children) {
      printNode(child);
    }
    std::cout << "} ";
  }
  else {
    std::cout << node->token.to_string() << "(" << node_type_to_string(node->value) << ") ";
  }
}

#endif // NODE_H