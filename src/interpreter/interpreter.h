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

  node *isRegisteredFunc(int scopeIndex, std::string function_name) {
    while (scopeIndex >= 0) {
      if (scopes[scopeIndex].functionScope.find(function_name) != scopes[scopeIndex].functionScope.end()) {
        return scopes[scopeIndex].functionScope[function_name];
      }
      scopeIndex--;
    }
    return nullptr;
  }

  std::vector<RuntimeResult> visit(node *_node, int scopeIndex = -1) {
    if (scopeIndex == -1) scopeIndex = scopes.size() - 1;
    switch (_node->value) {
      case node_type::_number: {
        return {RuntimeResult().success(_node->token.get_value())};
      }
      case node_type::_boolean: {
        return {RuntimeResult().success(_node->token.get_value())};
      }
      case node_type::_str: {
        return {RuntimeResult().success(_node->token.get_value())};
      }
      case node_type::_nullnode: {
        return {RuntimeResult().success(_node->token.get_value())};
      }
      case node_type::_unary_operator: {
        auto right = visit(_node->right);
        if (right[right.size() - 1].has_error()) return right;
        if (_node->token.get_name() == KEY_ADD) {
          return {RuntimeResult().success(right[right.size() - 1].get_value())};
        } else if (_node->token.get_name() == KEY_SUB) {
          return {RuntimeResult().success(neg(right[right.size() - 1].get_value()))};
        } else if (_node->token.get_name() == KEY_NOT) {
          return {RuntimeResult().success(logic_not(right[right.size() - 1].get_value()))};
        }
      }
      case node_type::_binary_operator: {
        auto left = visit(_node->left);
        if (left[left.size() - 1].has_error()) return left;
        auto right = visit(_node->right);
        if (right[right.size() - 1].has_error()) return right;
        if (_node->token.get_name() == KEY_ADD) {
          return {RuntimeResult().success(add(left[left.size() - 1].get_value(), right[right.size() - 1].get_value()))};
        } else if (_node->token.get_name() == KEY_SUB) {
          return {RuntimeResult().success(sub(left[left.size() - 1].get_value(), right[right.size() - 1].get_value()))};
        } else if (_node->token.get_name() == KEY_MUL) {
          return {RuntimeResult().success(mul(left[left.size() - 1].get_value(), right[right.size() - 1].get_value()))};
        } else if (_node->token.get_name() == KEY_DIV) {
          return {RuntimeResult().success(div(left[left.size() - 1].get_value(), right[right.size() - 1].get_value()))};
        } else if (_node->token.get_name() == KEY_MOD) {
          return {RuntimeResult().success(mod(left[left.size() - 1].get_value(), right[right.size() - 1].get_value()))};
        } else if (_node->token.get_name() == KEY_POW) {
          return {RuntimeResult().success(pow(left[left.size() - 1].get_value(), right[right.size() - 1].get_value()))};
        } else if (_node->token.get_name() == KEY_EE) {
          return {RuntimeResult().success(ee(left[left.size() - 1].get_value(), right[right.size() - 1].get_value()))};
        } else if (_node->token.get_name() == KEY_NE) {
          return {RuntimeResult().success(ne(left[left.size() - 1].get_value(), right[right.size() - 1].get_value()))};
        } else if (_node->token.get_name() == KEY_LT) {
          return {RuntimeResult().success(lt(left[left.size() - 1].get_value(), right[right.size() - 1].get_value()))};
        } else if (_node->token.get_name() == KEY_GT) {
          return {RuntimeResult().success(gt(left[left.size() - 1].get_value(), right[right.size() - 1].get_value()))};
        } else if (_node->token.get_name() == KEY_LE) {
          return {RuntimeResult().success(le(left[left.size() - 1].get_value(), right[right.size() - 1].get_value()))};
        } else if (_node->token.get_name() == KEY_GE) {
          return {RuntimeResult().success(ge(left[left.size() - 1].get_value(), right[right.size() - 1].get_value()))};
        } else if (_node->token.get_name() == KEY_AND) {
          return {RuntimeResult().success(logic_and(left[left.size() - 1].get_value(), right[right.size() - 1].get_value()))};
        } else if (_node->token.get_name() == KEY_OR) {
          return {RuntimeResult().success(logic_or(left[left.size() - 1].get_value(), right[right.size() - 1].get_value()))};
        }
      }
      case node_type::_variable_assign: {
        if (scopes[scopeIndex].localScope.find(_node->token.get_name()) != scopes[scopeIndex].localScope.end()) {
          return {RuntimeResult().failure(Error("Name Error", "Variable is already defined"))};
        }
        auto value = visit(_node->left);
        if (value[value.size() - 1].has_error()) return value;
        // std::cout << (*_scope)->localScope.size() << std::endl;
        scopes[scopeIndex].localScope[_node->token.get_name()] = value[value.size() - 1].get_value();
        // std::cout << (*_scope)->localScope.size() << std::endl;
        return {RuntimeResult().success(value[value.size() - 1].get_value())};
      }
      case node_type::_variable_reassign: {
        int itr = scopeIndex;
        while (itr >= 0 && scopes[itr].localScope.find(_node->token.get_name()) == scopes[itr].localScope.end()) {
          itr--;
        }
        if (itr == -1) {
          return {RuntimeResult().failure(Error("Name Error", "Unidentified token '" + _node->token.get_name() + "'"))};
        }
        auto value = visit(_node->left);
        if (value[value.size() - 1].has_error()) return value;
        // std::cout << (*_scope)->localScope.size() << std::endl;
        data _res = value[value.size() - 1].get_value();
        scopes[itr].localScope[_node->token.get_name()] = _res;
        // std::cout << (*_scope)->localScope.size() << std::endl;
        return {RuntimeResult().success(_res)};
      }
      case node_type::_variable_access: {
        int itr = scopeIndex;
        while (itr >= 0 && scopes[itr].localScope.find(_node->token.get_name()) == scopes[itr].localScope.end()) {
          itr--;
        }
        if (itr == -1)
          return {RuntimeResult().failure(Error("Name Error", "Variable is not defined"))};

        auto value = scopes[itr].localScope.find(_node->token.get_name())->second;
        return {RuntimeResult().success(value)};
      }
      case node_type::_function_block: {
        auto result = std::vector<RuntimeResult>();
        for (auto child : _node->children) {
          if (child->value == node_type::_return) {
            auto _res = visit(child->left);
            if (_res[_res.size() - 1].has_error()) return {RuntimeResult().failure(_res[_res.size() - 1].get_error())};
            return {RuntimeResult().success(_res[0].get_value())};
          }
          auto tempRes = visit(child);
          for (auto &_r : tempRes) {
            if (_r.has_error()) return result;
          }
        }
        type_value v;
        v._int = 0;
        return {RuntimeResult().success(data(types::_null, v))};
      }
      case node_type::_code_block: {
        scope new_scope;
        scopes.push_back(new_scope);
        int scopeIndex = scopes.size() - 1;
        auto result = std::vector<RuntimeResult>();
        for (auto child : _node->children) {
          auto tempRes = visit(child, scopeIndex);
          for (auto &_r : tempRes) {
            result.emplace_back(_r);
            if (_r.has_error()) return result;
          }
          if (result[result.size() - 1].has_error()) return result;
        }
        scopes.erase(scopes.begin() + scopeIndex);
        return result;
      }
      case node_type::_if_else: {
        auto _cond = visit(_node->left);
        data _res = type_cast(_cond[_cond.size() - 1].get_value(), _bool);
        if (_res.value._bool) {
          return visit(_node->right);
        }
        for (auto elif : _node->children) {
          if (elif->token.get_name() == KEY_ELSE) return visit(elif->right);
          _cond = visit(elif->left);
          _res = type_cast(_cond[_cond.size() - 1].get_value(), _bool);
          if (_res.value._bool) {
            return visit(elif->right);
          }
        }
        return {RuntimeResult()};
      }
      case node_type::_while_loop: {
        auto _cond = visit(_node->left);
        data _res = type_cast(_cond[_cond.size() - 1].get_value(), _bool);
        while (_res.value._bool) {
          auto _r = visit(_node->right);
          if (_r[_r.size() - 1].has_error()) return _r;
          _cond = visit(_node->left);
          _res = type_cast(_cond[_cond.size() - 1].get_value(), _bool);
        }
        return {RuntimeResult()};
      }
      case node_type::_function_definition: {
        bool _r = registerFunction(scopeIndex, _node->token.get_name(), &_node);
        if (!_r) return {RuntimeResult().failure(Error("Name Error", "Function is already defined"))};
        return {RuntimeResult()};
      }
      case node_type::_function_call: {
        node *_func = isRegisteredFunc(scopeIndex, _node->token.get_name());
        if (_func == nullptr) {
          return {RuntimeResult().failure(Error("Name Error", "Function is not defined"))};
        }
        if (_func->args.size() != _node->children.size()) {
          return {RuntimeResult().failure(Error("Name Error", "Function arguments are not matching"))};
        }
        scope new_scope;
        scopes.push_back(new_scope);
        int scopeIndex = scopes.size() - 1;
        for(int i = 0; i < _func->args.size(); i++) {
          auto _res = visit(_node->children[i], scopeIndex);
          if (_res[_res.size() - 1].has_error()) return _res;
          scopes[scopeIndex].localScope[_func->args[i].get_name()] = _res[_res.size() - 1].get_value();
        }
        auto _res = visit(_func->left, scopeIndex);
        if (_res[_res.size() - 1].has_error()) return _res;
        scopes.erase(scopes.begin() + scopeIndex);
        return _res;
      }
      default: {
        return {RuntimeResult().failure(Error("Unknown Error", "Unexpected Node"))};
      }
    }
  }
};

#endif