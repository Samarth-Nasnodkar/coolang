#include "node_type.h"
#include "../lexer/token.h"

#ifndef NODE_H
#define NODE_H

struct node {
  Token token;
  node_type value;
  struct node *left;
  struct node *right;
};

void printNode(node *node) {
  if (node->value == node_type::_binary_operator) {
    std::cout << "(";
    printNode(node->left);
    std::cout << " " << node->token.get_name() << " ";
    printNode(node->right);
    std::cout << ")";
  } else {
    std::cout << node->token.to_string();
  }
}

#endif // NODE_H