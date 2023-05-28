#include "../parser/parser.h"
#include "runtime_result.h"
#include "../variables/operations.h"
#include "scope.h"
#include "type_casting.h"

#ifndef INTERPRETER_H
#define INTERPRETER_H

class Interpreter {
  std::vector<scope> scopes;
  scope globalScope;
public:
  Interpreter() {
    globalScope = scope();
    globalScope.index = 0;
    globalScope.function = false;
    scopes.push_back(globalScope);
  }

  bool registerFunction(int scopeIndex, std::string function_name, node **function_node) {
    if (scopes[scopeIndex].localScope.find(function_name) != scopes[scopeIndex].localScope.end()) return false;
    scopes[scopeIndex].functionScope[function_name] = *function_node;
    return true;
  } 

  bool registerVariable(int scopeIndex, std::string variable_name, data val) {
    if (scopes[scopeIndex].localScope.find(variable_name) != scopes[scopeIndex].localScope.end()) return false;
    scopes[scopeIndex].localScope[variable_name] = val;
    return true;
  }

  int isRegisteredFunc(int scopeIndex, std::string function_name) {
    while (scopeIndex >= 0) {
      if (scopes[scopeIndex].functionScope.find(function_name) != scopes[scopeIndex].functionScope.end()) {
        return scopeIndex;
      }
      scopeIndex--;
    }
    return -1;
  }

  bool isFunction(int scopeIndex) {
    while (scopeIndex >= 0) {
      if (scopes[scopeIndex].function) return true;
      --scopeIndex;
    }
    return false;
  }

  RuntimeResult visit(node *_node, int scopeIndex = -1) {
    if (scopeIndex == -1) scopeIndex = scopes.size() - 1;
    switch (_node->value) {
      case node_type::_number: {
        return RuntimeResult().success(_node->token.get_value());
      }
      case node_type::_boolean: {
        return RuntimeResult().success(_node->token.get_value());
      }
      case node_type::_str: {
        return RuntimeResult().success(_node->token.get_value());
      }
      case node_type::_nullnode: {
        return RuntimeResult().success(_node->token.get_value());
      }
      case node_type::_unary_operator: {
        auto right = visit(_node->right);
        if (right.has_error()) return right;
        if (_node->token.get_name() == KEY_ADD) {
          return {RuntimeResult().success(right.get_value())};
        } else if (_node->token.get_name() == KEY_SUB) {
          return {RuntimeResult().success(neg(right.get_value()))};
        } else if (_node->token.get_name() == KEY_NOT) {
          return {RuntimeResult().success(logic_not(right.get_value()))};
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
        } else if (_node->token.get_name() == KEY_EE) {
          return RuntimeResult().success(ee(left.get_value(), right.get_value()));
        } else if (_node->token.get_name() == KEY_NE) {
          return RuntimeResult().success(ne(left.get_value(), right.get_value()));
        } else if (_node->token.get_name() == KEY_LT) {
          return RuntimeResult().success(lt(left.get_value(), right.get_value()));
        } else if (_node->token.get_name() == KEY_GT) {
          return RuntimeResult().success(gt(left.get_value(), right.get_value()));
        } else if (_node->token.get_name() == KEY_LE) {
          return RuntimeResult().success(le(left.get_value(), right.get_value()));
        } else if (_node->token.get_name() == KEY_GE) {
          return RuntimeResult().success(ge(left.get_value(), right.get_value()));
        } else if (_node->token.get_name() == KEY_AND) {
          return RuntimeResult().success(logic_and(left.get_value(), right.get_value()));
        } else if (_node->token.get_name() == KEY_OR) {
          return RuntimeResult().success(logic_or(left.get_value(), right.get_value()));
        }
      }
      case node_type::_variable_assign: {
        if (scopes[scopeIndex].localScope.find(_node->token.get_name()) != scopes[scopeIndex].localScope.end()) {
          return RuntimeResult().failure(Error("Name Error", "Variable is already defined"));
        }
        auto value = visit(_node->left);
        if (value.has_error()) return value;
        // std::cout << (*_scope)->localScope.size() << std::endl;
        scopes[scopeIndex].localScope[_node->token.get_name()] = value.get_value();
        // std::cout << (*_scope)->localScope.size() << std::endl;
        return RuntimeResult().success(value.get_value());
      }
      case node_type::_variable_reassign: {
        int itr = scopeIndex;
        while (itr >= 0 && scopes[itr].localScope.find(_node->token.get_name()) == scopes[itr].localScope.end()) {
          itr--;
        }
        if (itr == -1) {
          return RuntimeResult().failure(Error("Name Error", "Unidentified token '" + _node->token.get_name() + "'"));
        }
        auto value = visit(_node->left);
        if (value.has_error()) return value;
        // std::cout << (*_scope)->localScope.size() << std::endl;
        data _res = value.get_value();
        scopes[itr].localScope[_node->token.get_name()] = _res;
        // std::cout << (*_scope)->localScope.size() << std::endl;
        return RuntimeResult().success(_res);
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
      case node_type::_return: {
        if (!isFunction(scopeIndex)) {
          return RuntimeResult().failure(Error("Invalid Syntax Error", "'return' outside function block"));
        }
        auto _res = visit(_node->left);
        _res.is_return = true;
        if (_res.has_error()) return {RuntimeResult().failure(_res.get_error())};
        if (_res.is_empty())
          return RuntimeResult().success(data(types::_null, type_value()));
        return RuntimeResult().success(_res.get_value());
      }
      case node_type::_function_block: {
        RuntimeResult result;
        for (auto child : _node->children) {
          if (child->value == node_type::_return) {
            // auto _res = visit(child->left);
            // if (_res.has_error()) return {RuntimeResult().failure(_res.get_error())};
            // return {RuntimeResult().success(_res[0].get_value())};
            auto _res = visit(child, scopeIndex);
            _res.is_return = true;
            return _res;
          }
          result = visit(child, scopeIndex);
          if (result.is_return) return result;
          if (result.has_error()) return result;
        }
        if (!result.is_empty()) return result;
        type_value v;
        v._int = 0;
        return RuntimeResult().success(data(types::_null, v));
      }
      case node_type::_code_block: {
        scope new_scope;
        int scopeIndex = scopes.size();
        new_scope.index = scopeIndex;
        scopes.push_back(new_scope);
        RuntimeResult result;
        for (auto child : _node->children) {
          if (child->value == node_type::_return) {
            auto _res = visit(child, scopeIndex);
            _res.is_return = true;
            return _res;
          }
          result = visit(child, scopeIndex);
          if (result.is_return) return result;
          if (result.has_error()) return result;
        }
        scopes.erase(scopes.begin() + scopeIndex);
        if (isFunction(scopeIndex - 1)) {
          if (result.is_empty())
            return RuntimeResult().success(data(types::_null, type_value()));
        }
        return result;
      }
      case node_type::_if_else: {
        auto _cond = visit(_node->left);
        if (_cond.has_error()) 
          return RuntimeResult().failure(_cond.get_error());
        data _res = type_cast(_cond.get_value(), _bool);
        if (_res.value._bool) {
          return visit(_node->right);
        }
        for (auto elif : _node->children) {
          if (elif->token.get_name() == KEY_ELSE) return visit(elif->right);
          _cond = visit(elif->left);
          if (_cond.has_error()) 
            return RuntimeResult().failure(_cond.get_error());
          _res = type_cast(_cond.get_value(), _bool);
          if (_res.value._bool) {
            return visit(elif->right);
          }
        }
        return RuntimeResult();
      }
      case node_type::_while_loop: {
        auto _cond = visit(_node->left);
        data _res = type_cast(_cond.get_value(), _bool);
        while (_res.value._bool) {
          auto _r = visit(_node->right);
          if (_r.has_error()) return _r;
          _cond = visit(_node->left);
          _res = type_cast(_cond.get_value(), _bool);
        }
        return RuntimeResult();
      }
      case node_type::_function_definition: {
        bool _r = registerFunction(scopeIndex, _node->token.get_name(), &_node);
        if (!_r) return RuntimeResult().failure(Error("Name Error", "Function is already defined"));
        return RuntimeResult();
      }
      case node_type::_function_call: {
        int _indx = isRegisteredFunc(scopeIndex, _node->token.get_name());
        if (_indx == -1) {
          return RuntimeResult().failure(Error("Name Error", "Function is not defined"));
        }
        node *_func = scopes[_indx].functionScope[_node->token.get_name()];
        if (_func->args.size() != _node->children.size()) {
          return RuntimeResult().failure(Error("Name Error", "Function arguments are not matching"));
        }
        scope new_scope;
        int scopeIndex = scopes.size();
        new_scope.index = scopeIndex;
        new_scope.function = true;
        scopes.push_back(new_scope);
        for(int i = 0; i < _func->args.size(); i++) {
          auto _res = visit(_node->children[i], scopeIndex);
          if (_res.has_error()) return _res;
          scopes[scopeIndex].localScope[_func->args[i].get_name()] = _res.get_value();
        }
        auto _res = visit(_func->left, scopeIndex);
        scopes.erase(scopes.begin() + scopeIndex);
        if (_res.has_error()) return _res;
        return _res;
      }
      default: {
        return RuntimeResult().failure(Error("Unknown Error", "Unexpected Node"));
      }
    }
  }
};

#endif