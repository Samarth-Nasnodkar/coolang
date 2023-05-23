#include "../parser/parser.h"
#include "runtime_result.h"
#include "../variables/operations.h"

#ifndef INTERPRETER_H
#define INTERPRETER_H

class Interpreter {
  std::map<std::string, data> scope;
public:
  Interpreter() {
    scope = std::map<std::string, data>();
  }

  RuntimeResult visit(node *_node) {
    switch (_node->value) {
      case node_type::_number: {
        return RuntimeResult().success(_node->token.get_value());
      }
      case node_type::_unary_operator: {
        auto right = visit(_node->right);
        if (right.has_error()) return right;
        if (_node->token.get_name() == KEY_ADD) {
          return RuntimeResult().success(right.get_value());
        } else if (_node->token.get_name() == KEY_SUB) {
          return RuntimeResult().success(neg(right.get_value()));
        }
      }
      case node_type::_binary_operator: {
        auto left = visit(_node->left);
        if (left.has_error()) return left;
        auto right = visit(_node->right);
        if (right.has_error()) return right;
        if (_node->token.get_name() == KEY_ADD) {
          return RuntimeResult().success(add(left.get_value(), right.get_value()));
        } else if (_node->token.get_name() == KEY_SUB) {
          return RuntimeResult().success(sub(left.get_value(), right.get_value()));
        } else if (_node->token.get_name() == KEY_MUL) {
          return RuntimeResult().success(mul(left.get_value(), right.get_value()));
        } else if (_node->token.get_name() == KEY_DIV) {
          return RuntimeResult().success(div(left.get_value(), right.get_value()));
        } else if (_node->token.get_name() == KEY_MOD) {
          return RuntimeResult().success(mod(left.get_value(), right.get_value()));
        } else if (_node->token.get_name() == KEY_POW) {
          return RuntimeResult().success(pow(left.get_value(), right.get_value()));
        }
      }
      case node_type::_variable_assign: {
        if (scope.find(_node->token.get_name()) != scope.end()) {
          return RuntimeResult().failure(Error("Name Error", "Variable is already defined"));
        }
        auto value = visit(_node->left);
        if (value.has_error()) return value;
        scope.insert(std::pair<std::string, data>(_node->token.get_name(), value.get_value()));
        return RuntimeResult().success(value.get_value());
      }
      case node_type::_variable_access: {
        if (scope.find(_node->token.get_name()) == scope.end()) {
          return RuntimeResult().failure(Error("Name Error", "Variable is not defined"));
        }
        auto value = scope.find(_node->token.get_name())->second;
        return RuntimeResult().success(value);
      }
      default: {
        return RuntimeResult().failure(Error("Invalid Syntax Error", "Expected Number"));
      }
    }
  }
};

#endif