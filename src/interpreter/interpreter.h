#include "../parser/parser.h"
#include "runtime_result.h"
#include "../variables/operations.h"

#ifndef INTERPRETER_H
#define INTERPRETER_H

class Interpreter {
public:
  Interpreter() {
  }

  RuntimeResult visit(node *_node) {
    switch (_node->value) {
      case node_type::_number: {
        return RuntimeResult().success(_node->token.get_value());
      }
      case node_type::_unary_operator: {
        auto right = visit(_node->right);
        if (right.has_error()) return right;
        if (_node->token.get_name() == "ADD") {
          return RuntimeResult().success(right.get_value());
        } else if (_node->token.get_name() == "SUB") {
          return RuntimeResult().success(neg(right.get_value()));
        }
      }
      case node_type::_binary_operator: {
        auto left = visit(_node->left);
        if (left.has_error()) return left;
        auto right = visit(_node->right);
        if (right.has_error()) return right;
        if (_node->token.get_name() == "ADD") {
          return RuntimeResult().success(add(left.get_value(), right.get_value()));
        } else if (_node->token.get_name() == "SUB") {
          return RuntimeResult().success(sub(left.get_value(), right.get_value()));
        } else if (_node->token.get_name() == "MUL") {
          return RuntimeResult().success(mul(left.get_value(), right.get_value()));
        } else if (_node->token.get_name() == "DIV") {
          return RuntimeResult().success(div(left.get_value(), right.get_value()));
        } else if (_node->token.get_name() == "MOD") {
          return RuntimeResult().success(mod(left.get_value(), right.get_value()));
        } else if (_node->token.get_name() == "POW") {
          return RuntimeResult().success(pow(left.get_value(), right.get_value()));
        }
      }
      default: {
        return RuntimeResult().failure(Error("Invalid Syntax Error", "Expected Number"));
      }
    }
  }
};

#endif