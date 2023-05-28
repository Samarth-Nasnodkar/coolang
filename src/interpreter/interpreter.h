#include "../parser/parser.h"
#include "runtime_result.h"
#include "../variables/operations.h"
#include "scope.h"
#include "type_casting.h"
#include "primitive_func.h"

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
    if (scopes[scopeIndex].localScope.find(function_name) != scopes[scopeIndex].localScope.end()) 
      return false;
    scopes[scopeIndex].functionScope[function_name] = *function_node;
    return true;
  } 

  bool registerVariable(int scopeIndex, std::string variable_name, data val) {
    if (scopes[scopeIndex].localScope.find(variable_name) != scopes[scopeIndex].localScope.end()) 
      return false;
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

  bool isPrimitive(std::string function_name) {
    for(int i = 0; i < sizeof(primitive_funcs) / sizeof(primitive_funcs[0]); i++) {
      if (primitive_funcs[i] == function_name) return true;
    }
    return false;
  }

  bool isFunction(int scopeIndex) {
    while (scopeIndex >= 0) {
      if (scopes[scopeIndex].function) return true;
      --scopeIndex;
    }
    return false;
  }

  std::pair<data, Error> handlePrimitive(node *func) {
    std::string func_name = func->token.get_name();
    if (func_name == "print") {
      int argc = func->children.size();
      data argv[argc];
      for(int i = 0; i < argc; i++) {
        auto _res = visit(func->children[i]);
        if (_res.has_error()) return {data(), _res.get_error()};
        argv[i] = _res.get_value();
      }
      return {print(argv, argc), Error()};
    }
    if (func_name == "type_of") {
      if (func->children.size() != 1) {
        return {data(), Error("Invalid Prameters", "Expected 1 got " + func->children.size())};
      } 
      auto v = visit(func->children[0]);
      if (v.has_error()) return {data(), v.get_error()};
      return {type_of(v.get_value()), Error()};
    }
    if (func_name == "len") {
      if (func->children.size() != 1) {
        return {data(), Error("Invalid Prameters", "Expected 1 got " + func->children.size())};
      } 
      auto v = visit(func->children[0]);
      if (v.has_error()) return {data(), v.get_error()};
      return {len(v.get_value()), Error()};
    }
    return {data(), Error()};
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
      case node_type::_list: {
        data *r = new data[_node->children.size()];
        for(int i = 0; i < _node->children.size(); i++) {
          auto _res = visit(_node->children[i]);
          if (_res.has_error()) return _res;
          r[i] = _res.get_value();
        }
        type_value v;
        int len = _node->children.size();
        v._list = new _llist{r, len};
        return RuntimeResult().success(data(types::_list_type, v));
      }
      case node_type::_index: {
        int itr = scopeIndex;
        while (itr >= 0 && scopes[itr].localScope.find(_node->token.get_name()) == scopes[itr].localScope.end()) {
          itr--;
        }
        if (itr == -1)
          return RuntimeResult().failure(Error("Name Error", "Variable is not defined"));
        
        auto list_var = scopes[itr].localScope[_node->token.get_name()];
        if (list_var._type != _list_type) {
          return RuntimeResult().failure(Error("Type Error", "Index operator used on " + type_of(list_var).to_string()));
        }
        auto _index = visit(_node->left);
        if (_index.has_error()) return _index;
        if (_index.get_value()._type != types::_int) {
          return RuntimeResult().failure(Error("Type Error", "Index operator used with " + type_of(_index.get_value()).to_string()));
        }
        int index = _index.get_value().value._int;
        if (index < 0 || index >= list_var.value._list->size) {
          return RuntimeResult().failure(Error("Index Error", "Index out of range"));
        }
        data *_data = (data*)list_var.value._list->_data;
        return RuntimeResult().success(_data[index]);
      }
      case node_type::_nullnode: {
        return RuntimeResult().success(_node->token.get_value());
      }
      case node_type::_binary_operator: {
        auto left = visit(_node->left);
        if (left.has_error()) return left;
        auto right = visit(_node->right);
        if (right.has_error()) return right;
        if (_node->token.get_name() == KEY_ADD) {
          auto r = add(left.get_value(), right.get_value());
          if (r.second.has_error()) return RuntimeResult().failure(r.second);
          return RuntimeResult().success(r.first);
        } else if (_node->token.get_name() == KEY_SUB) {
          auto r = sub(left.get_value(), right.get_value());
          if (r.second.has_error()) return RuntimeResult().failure(r.second);
          return RuntimeResult().success(r.first);
        } else if (_node->token.get_name() == KEY_MUL) {
          auto r = mul(left.get_value(), right.get_value());
          if (r.second.has_error()) return RuntimeResult().failure(r.second);
          return RuntimeResult().success(r.first);
        } else if (_node->token.get_name() == KEY_DIV) {
          auto r = div(left.get_value(), right.get_value());
          if (r.second.has_error()) return RuntimeResult().failure(r.second);
          return RuntimeResult().success(r.first);
        } else if (_node->token.get_name() == KEY_MOD) {
          auto r = mod(left.get_value(), right.get_value());
          if (r.second.has_error()) return RuntimeResult().failure(r.second);
          return RuntimeResult().success(r.first);
        } else if (_node->token.get_name() == KEY_POW) {
          auto r = pow(left.get_value(), right.get_value());
          if (r.second.has_error()) return RuntimeResult().failure(r.second);
          return RuntimeResult().success(r.first);
        } else if (_node->token.get_name() == KEY_EE) {
          auto r = ee(left.get_value(), right.get_value());
          if (r.second.has_error()) return RuntimeResult().failure(r.second);
          return RuntimeResult().success(r.first);
        } else if (_node->token.get_name() == KEY_NE) {
          auto r = ne(left.get_value(), right.get_value());
          if (r.second.has_error()) return RuntimeResult().failure(r.second);
          return RuntimeResult().success(r.first);
        } else if (_node->token.get_name() == KEY_LT) {
          auto r = lt(left.get_value(), right.get_value());
          if (r.second.has_error()) return RuntimeResult().failure(r.second);
          return RuntimeResult().success(r.first);
        } else if (_node->token.get_name() == KEY_GT) {
          auto r = gt(left.get_value(), right.get_value());
          if (r.second.has_error()) return RuntimeResult().failure(r.second);
          return RuntimeResult().success(r.first);
        } else if (_node->token.get_name() == KEY_LE) {
          auto r = le(left.get_value(), right.get_value());
          if (r.second.has_error()) return RuntimeResult().failure(r.second);
          return RuntimeResult().success(r.first);
        } else if (_node->token.get_name() == KEY_GE) {
          auto r = ge(left.get_value(), right.get_value());
          if (r.second.has_error()) return RuntimeResult().failure(r.second);
          return RuntimeResult().success(r.first);
        } else if (_node->token.get_name() == KEY_AND) {
          auto r = logic_and(left.get_value(), right.get_value());
          if (r.second.has_error()) return RuntimeResult().failure(r.second);
          return RuntimeResult().success(r.first);
        } else if (_node->token.get_name() == KEY_OR) {
          auto r = logic_or(left.get_value(), right.get_value());
          if (r.second.has_error()) return RuntimeResult().failure(r.second);
          return RuntimeResult().success(r.first);
        }
        return RuntimeResult().failure(Error("Unknown Binary Operator", "Unknown Binary Operator Found"));
      }
      case node_type::_unary_operator: {
        auto right = visit(_node->right);
        if (right.has_error()) return right;
        if (_node->token.get_name() == KEY_ADD) {
          return {RuntimeResult().success(right.get_value())};
        } else if (_node->token.get_name() == KEY_SUB) {
          auto r = neg(right.get_value());
          if (r.second.has_error()) return RuntimeResult().failure(r.second);
          return RuntimeResult().success(r.first);
        } else if (_node->token.get_name() == KEY_NOT) {
          auto r = logic_not(right.get_value());
          if (r.second.has_error()) return RuntimeResult().failure(r.second);
          return RuntimeResult().success(r.first);
        }
        return RuntimeResult().failure(Error("Unknown Unary Operator", "Unknown Unary Operator Found"));
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
        data _res = value.get_value();
        scopes[itr].localScope[_node->token.get_name()] = _res;
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
          if (isPrimitive(_node->token.get_name())) {
            auto _res = handlePrimitive(_node);
            if (_res.second.has_error()) return RuntimeResult().failure(_res.second);
            return RuntimeResult().success(_res.first);
          }
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