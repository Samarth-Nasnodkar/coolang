#include "../parser/parser.h"
#include "runtime_result.h"
#include "../variables/operations.h"
#include "scope.h"

#ifndef INTERPRETER_H
#define INTERPRETER_H

class Interpreter {
  std::vector<scope> scopes;
  scope globalScope;
public:
  Interpreter() {
    globalScope = {0};
    scopes.push_back(globalScope);
  }

  RuntimeResult visit(node *_node, int scopeIndex = -1) {
    if (scopeIndex == -1) scopeIndex = scopes.size() - 1;
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
        if (scopes[scopeIndex].localScope.find(_node->token.get_name()) != scopes[scopeIndex].localScope.end()) {
          return RuntimeResult().failure(Error("Name Error", "Variable is already defined"));
        }
        auto value = visit(_node->left);
        if (value.has_error()) return value;
        // std::cout << (*_scope)->localScope.size() << std::endl;
        scopes[scopeIndex].localScope.insert(std::pair<std::string, data>(_node->token.get_name(), value.get_value()));
        // std::cout << (*_scope)->localScope.size() << std::endl;
        return RuntimeResult().success(value.get_value());
      }
      case node_type::_variable_access: {
        int itr = scopeIndex;
        while (itr >= 0 && scopes[itr].localScope.find(_node->token.get_name()) == scopes[itr].localScope.end()) {
          itr--;
        }
        if (itr == -1)
          return RuntimeResult().failure(Error("Name Error", "Variable is not defined"));

        auto value = scopes[itr].localScope.find(_node->token.get_name())->second;
        return RuntimeResult().success(value);
      }
      case node_type::_code_block: {
        scope new_scope;
        scopes.push_back(new_scope);
        int scopeIndex = scopes.size() - 1;
        auto result = RuntimeResult();
        for (auto child : _node->children) {
          result = visit(child, scopeIndex);
          if (result.has_error()) return result;
        }
        scopes.erase(scopes.begin() + scopeIndex);
        return result;
      }
      default: {
        return RuntimeResult().failure(Error("Invalid Syntax Error", "Expected Number"));
      }
    }
  }
};

#endif