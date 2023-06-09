#include "../parser/parser.h"
#include "runtime_result.h"
#include "../variables/operations.h"
#include "scope.h"
#include "type_casting.h"
// #include "primitive_func.h"
#include "get_var_value.h"
#include "runtime_ops.h"

#ifndef INTERPRETER_H
#define INTERPRETER_H

class Interpreter {
  std::vector<scope> scopes;
  scope globalScope;
  std::string primitive_funcs[4] = {
    "println",
    "print",
    "type_of",
    "len",
  };
public:
  Interpreter() {
    globalScope = scope();
    globalScope.index = 0;
    globalScope.function = false;
    scopes.push_back(globalScope);
  }

  bool registerFunction(int scopeIndex, std::string function_name, node **function_node) {
    if (getVarValue(scopes[scopeIndex], function_name).first != runtime_type::__ne) 
      return false;
    scopes[scopeIndex].localScope[function_name] = {__func, *function_node};
    return true;
  } 

  bool registerObject(int scopeIndex, std::string object_name, node **object_node) {
    if (scopes[scopeIndex].localScope.find(object_name) != scopes[scopeIndex].localScope.end()) 
      return false;
    scopes[scopeIndex].objectDefinition[object_name] = (*object_node)->left;
    return true;
  } 

  node *fetchMethodByName(node *obj_def, std::string method_name) {
    if (obj_def == nullptr) return nullptr;
    for (auto child : obj_def->children) {
      if (child->value == _method_call) {
        if (child->token.get_name() == method_name)
          return child;
      }
    }
    return nullptr;
  }

  std::pair<__runtime_var, Error> print(std::pair<runtime_type, void*> a[], int n) {
    for(int i = 0; i < n; ++i) {
      // if (a[i].first != __var) {
      //   std::cout << "TypeError: print() only accepts variables" << std::endl;
      //   exit(1);
      // }
      if (a[i].first == __object) {
        auto __obj = (obj *)a[i].second;
        auto __obj_def = __obj->_def;
        auto _str_method = fetchMethodByName(__obj_def, "to_string");
        if (_str_method != nullptr) {
          auto __str = _str_method;
          auto __str_res = visit(__str, -1, &__obj);
          if (__str_res.has_error()) return {{__ne, nullptr}, __str_res.get_error()};
          auto __str_val = __str_res.get_value();
          if (__str_val.first != __var || ((data *)__str_val.second)->_type != _string) {
            return {{__ne, nullptr}, Error("TypeError", "to_string() does not return a str")};
          }
          std::string _to_str = ((data *)__str_val.second)->value._string;
          std::cout << _to_str;
          if (i != n - 1)
            std::cout << " ";
          continue;
        } else {
          std::cout << "<Instance of " << __obj->_type << " at " << __obj << ">";
          if (i != n - 1)
            std::cout << " ";
          continue;
        }
      }
      if (a[i].first == __func) {
        std::cout << "function.__" + ((node *)a[i].second)->token.get_name() + "__";
        if (i != n - 1)
          std::cout << " ";
        continue;
      }
      if (a[i].first == __ne) {
        std::cout << "undefined";
        if (i != n - 1)
          std::cout << " ";
        continue;
      }
      std::cout << ((data *)a[i].second)->to_string();
      if (i != n - 1)
        std::cout << " ";
    }
    return {{__var, new data{_null, type_value()}}, Error()};
  }

  std::pair<__runtime_var, Error> println(std::pair<runtime_type, void*> a[], int n) {
    auto _res = print(a, n);
    if (_res.second.has_error()) return _res;
    std::cout << std::endl;
    return {{__var, new data{_null, type_value()}}, Error()};
  }

  __runtime_var len(__runtime_var a) {
    if (a.first != __var) {
      std::cout << "TypeError: len() only accepts variables" << std::endl;
      exit(1);
    }
    data _a = *((data *)a.second);
    if (_a._type == types::_string) {
      int _len = strlen(_a.value._string);
      type_value v;
      v._int = _len;
      return {__var, new data(types::_int, v)};
    } else if (_a._type == types::_list_type) {
      int _len = _a.value._list->size;
      type_value v;
      v._int = _len;
      return {__var, new data(types::_int, v)};
    } else {
      std::cout << "TypeError: len() only accepts strings and lists" << std::endl;
      exit(1);
    }
  }

  __runtime_var type_of(__runtime_var a) {
    type_value v;
    std::string _type_str;
    if (a.first == __var) {
      data _a = *((data *)a.second);
      _type_str = types_str[_a._type];
      v._string = new char[_type_str.length() + 1];
    } else if (a.first == __object) {
      obj _a = *((obj *)a.second);
      _type_str = _a._type;
      v._string = new char[_type_str.length() + 1];
    } else if (a.first == __func) {
      _type_str = "function.__" + ((node *)a.second)->token.get_name() + "__";
      v._string = new char[_type_str.length() + 1];
    } else {
      _type_str = "undefined";
    }
    strncpy(v._string, _type_str.c_str(), _type_str.length() + 1);
    return {__var, new data(types::_string, v)};
  }

  std::pair<obj *, Error> instantiateObject(int scopeIndex, node *_node, obj **payload = nullptr, std::vector<node *> __args = {}) {
    while (scopeIndex >= 0) {
      if (scopes[scopeIndex].objectDefinition.find(_node->token.get_name()) != scopes[scopeIndex].objectDefinition.end()) {
        break;
      }
      scopeIndex--;
    }
    if (scopeIndex < 0) return {nullptr, Error("Runtime Error", "Object " + _node->token.get_name() + " is not defined")};
    obj *_obj = new obj();
    _obj->_type = std::move(_node->token.get_name());
    _obj->_name = std::move(_node->token.get_name());
    node *_obj_def = scopes[scopeIndex].objectDefinition[_obj->_type];
    _obj->_def = _obj_def;
    node *_constructor = nullptr;
    for(int i = 0; i < _obj_def->children.size(); i++) {
      node *_field = _obj_def->children[i];
      std::string field_name = _field->token.get_name();
      if (field_name == KEY_CONSTRUCTOR) _constructor = _field;
      if (_field->value == node_type::_field_assign) {
        if (_field->left == nullptr) {
          (_obj->_data)[field_name] = {__var, new data{types::_null, type_value()}};
        } else {
          auto _res = visit(_field->left, scopeIndex, payload, __args);
          if (_res.has_error()) return {nullptr, _res.get_error()};
          (_obj->_data)[field_name] = _res.get_value();
        }
      }
    }
    if (_constructor != nullptr) {
      auto _res = visit(_constructor, scopeIndex, &_obj, _node->children);
      if (_res.has_error()) {
        return {nullptr, _res.get_error()};
      }
      return {_obj, Error()};
    } else {
      if (_node->children.size() > 0) {
        return {nullptr, Error("Runtime Error", "Object " + _node->token.get_name() + " has no constructor")};
      }
      return {_obj, Error()};
    }
  }

  bool registerVariable(int scopeIndex, std::string variable_name, data val) {
    // if (scopes[scopeIndex].localScope.find(variable_name) != scopes[scopeIndex].localScope.end()) 
    if (getVarValue(scopes[scopeIndex], variable_name).first != runtime_type::__var) 
      return false;
    scopes[scopeIndex].localScope[variable_name] = {__var, new data(val)};
    return true;
  }


  int isRegisteredFunc(int scopeIndex, std::string function_name) {
    while (scopeIndex >= 0) {
      if (getVarValue(scopes[scopeIndex], function_name).first == runtime_type::__func) {
        return scopeIndex;
      }
      scopeIndex--;
    }
    return -1;
  }

  int isDefinedObj(int scopeIndex, std::string object_name) {
    while (scopeIndex >= 0) {
      if (scopes[scopeIndex].objectDefinition.find(object_name) != scopes[scopeIndex].objectDefinition.end()) {
        return scopeIndex;
      }
      scopeIndex--;
    }
    return -1;
  }

  int isRegisteredObj(int scopeIndex, std::string object_name) {
    while (scopeIndex >= 0) {
      if (getVarValue(scopes[scopeIndex], object_name).first == runtime_type::__object) {
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

  std::pair<__runtime_var, Error> handlePrimitive(node *func, int scopeIndex, obj **payload = nullptr, std::vector<node *> __args = {}) {
    std::string func_name = func->token.get_name();
    if (func_name == "print") {
      int argc = func->children.size();
      __runtime_var argv[argc];
      for(int i = 0; i < argc; i++) {
        auto _res = visit(func->children[i], scopeIndex, payload, __args);
        if (_res.has_error()) return {{__ne, nullptr}, _res.get_error()};
        argv[i] = _res.get_value();
      }
      auto __res = print(argv, argc);
      if (__res.second.has_error()) return {{__ne, nullptr}, __res.second};
      return {__res.first, Error()};
    }
    if (func_name == "println") {
      int argc = func->children.size();
      __runtime_var argv[argc];
      for(int i = 0; i < argc; i++) {
        auto _res = visit(func->children[i], scopeIndex, payload, __args);
        if (_res.has_error()) return {{__ne, nullptr}, _res.get_error()};
        argv[i] = _res.get_value();
      }
      auto __res = println(argv, argc);
      if (__res.second.has_error()) {
        return {{__ne, nullptr}, __res.second};
      }
      return {__res.first, Error()};
    }
    if (func_name == "type_of") {
      if (func->children.size() != 1) {
        return {{__ne, nullptr}, Error("Invalid Prameters", "Expected 1 got " + func->children.size())};
      }
      auto v = visit(func->children[0], scopeIndex, payload);
      if (v.has_error()) return {{__ne, nullptr}, v.get_error()};
      // if (v.get_value().first == __object) {
      //   type_value _tv;
      //   _tv._string = new char[v.get_obj()->_type.length() + 1];
      //   strncpy(_tv._string, v.get_obj()->_type.c_str(), v.get_obj()->_type.length() + 1);
      //   return {{__var, new data{types::_string, _tv}}, Error()};
      // }
      return {type_of(v.get_value()), Error()};
    }
    if (func_name == "len") {
      if (func->children.size() != 1) {
        return {{__ne, nullptr}, Error("Invalid Prameters", "Expected 1 got " + func->children.size())};
      } 
      auto v = visit(func->children[0], scopeIndex, payload);
      if (v.has_error()) return {{__ne, nullptr}, v.get_error()};
      return {len(v.get_value()), Error()};
    }
    return {{__ne, nullptr}, Error()};
  }

  RuntimeResult visit(node *_node, int scopeIndex = -1, obj **payload = nullptr, std::vector<node *> __args = std::vector<node *>()) {
    if (_node != nullptr)
    if (scopeIndex == -1) scopeIndex = scopes.size() - 1;
    switch (_node->value) {
      case node_type::_str:
      case node_type::_boolean:
      case node_type::_number: {
        return RuntimeResult().success({__var, new data{_node->token.get_value()}});
      }
      // case node_type::_boolean: {
      //   return RuntimeResult().success(_node->token.get_value());
      // }
      // case node_type::_str: {
      //   return RuntimeResult().success(_node->token.get_value());
      // }
      case node_type::_list: {
        __runtime_var *r = new __runtime_var[_node->children.size()];
        for(int i = 0; i < _node->children.size(); i++) {
          auto _res = visit(_node->children[i], scopeIndex, payload, __args);
          if (_res.has_error()) return _res;
          r[i] = _res.get_value();
        }
        type_value v;
        int len = _node->children.size();
        v._list = new _llist{r, len};
        return RuntimeResult().success({__var, new data(types::_list_type, v)});
      }
      case node_type::_method_call: {
        if (payload == nullptr) {
          return RuntimeResult().failure(Error("Invalid Object", "Object is null"));
        }
        if (_node->args.size() != __args.size()) {
          return RuntimeResult().failure(Error("Invalid Prameters", "Expected " + std::to_string(_node->args.size()) + " got " + std::to_string(__args.size())));
        }
        scope new_scope;
        int scopeIndex = scopes.size();
        new_scope.index = scopeIndex;
        new_scope.function = true;
        scopes.push_back(new_scope);
        for(int i = 0; i < _node->args.size(); i++) {
          auto _res = visit(__args[i], scopeIndex, payload, __args);
          if (_res.has_error()) return _res;
          if (_res.get_value().first == __object) {
            ((obj *)_res.get_value().second)->_name = _node->args[i].get_name();
          }
          scopes[scopeIndex].localScope[_node->args[i].get_name()] = _res.get_value();
        }
        auto _res = visit(_node->left, scopeIndex, payload, __args);
        scopes.erase(scopes.begin() + scopeIndex);
        if (_res.has_error()) return _res;
        return _res;
      }
      case node_type::_index: {
        int itr = scopeIndex;
        // while (itr >= 0 && scopes[itr].localScope.find(_node->token.get_name()) == scopes[itr].localScope.end()) {
        auto __varVal = getVarValue(scopes[itr], _node->token.get_name());
        while (itr >= 0 && __varVal.first == runtime_type::__ne) {
          itr--;
          __varVal = getVarValue(scopes[itr], _node->token.get_name());
        }
        if (itr < 0)
          return RuntimeResult().failure(Error("Name Error", "Variable is not defined"));
        
        if (__varVal.first != __var) {
          return RuntimeResult().failure(Error("Type Error", "Index operator used on " + ((data *)type_of(__varVal).second)->to_string()));
        }
        auto _index = visit(_node->left, scopeIndex, payload, __args);
        if (_index.has_error()) return _index;
        if (_index.get_value().first != __var)
          return RuntimeResult().failure(Error("Type Error", "Index operator only accepts int"));
        auto __v = _index.get_value();
        if (__v.first != __var)
          return RuntimeResult().failure(Error("Type Error", "Index operator only accepts int"));
        if (((data *)__v.second)->_type != types::_int) {
          return RuntimeResult().failure(Error("Type Error", "Index operator used with " + ((data *)type_of(__v).second)->to_string()));
        }
        auto list_var = (data *)__varVal.second;
        int index = ((data *)__v.second)->value._int;
        if (index < 0 || index >= list_var->value._list->size) {
          return RuntimeResult().failure(Error("Index Error", "Index out of range"));
        }
        __runtime_var *_data = (__runtime_var*)(list_var->value._list->_data);
        // return RuntimeResult().success({__var, &_data[index]});
        return RuntimeResult().success(_data[index]);
      }
      case node_type::_nullnode: {
        return RuntimeResult().success({__var, new data{_node->token.get_value()}});
      }
      case node_type::_binary_operator: {
        auto left = visit(_node->left, scopeIndex, payload, __args);
        if (left.has_error()) return left;
        auto right = visit(_node->right, scopeIndex, payload, __args);
        if (right.has_error()) return right;
        if (_node->token.get_name() == KEY_ADD) {
          auto r = binaryAdd(left.get_value(), right.get_value());
          if (r.second.has_error()) return RuntimeResult().failure(r.second);
          return RuntimeResult().success(r.first);
        } else if (_node->token.get_name() == KEY_SUB) {
          auto r = binarySub(left.get_value(), right.get_value());
          if (r.second.has_error()) return RuntimeResult().failure(r.second);
          return RuntimeResult().success(r.first);
        } else if (_node->token.get_name() == KEY_MUL) {
          auto r = binaryMul(left.get_value(), right.get_value());
          if (r.second.has_error()) return RuntimeResult().failure(r.second);
          return RuntimeResult().success(r.first);
        } else if (_node->token.get_name() == KEY_DIV) {
          auto r = binaryDiv(left.get_value(), right.get_value());
          if (r.second.has_error()) return RuntimeResult().failure(r.second);
          return RuntimeResult().success(r.first);
        } else if (_node->token.get_name() == KEY_MOD) {
          auto r = binaryMod(left.get_value(), right.get_value());
          if (r.second.has_error()) return RuntimeResult().failure(r.second);
          return RuntimeResult().success(r.first);
        } else if (_node->token.get_name() == KEY_POW) {
          auto r = binaryPow(left.get_value(), right.get_value());
          if (r.second.has_error()) return RuntimeResult().failure(r.second);
          return RuntimeResult().success(r.first);
        } else if (_node->token.get_name() == KEY_EE) {
          auto r = binaryEe(left.get_value(), right.get_value());
          if (r.second.has_error()) return RuntimeResult().failure(r.second);
          return RuntimeResult().success(r.first);
        } else if (_node->token.get_name() == KEY_NE) {
          auto r = binaryNe(left.get_value(), right.get_value());
          if (r.second.has_error()) return RuntimeResult().failure(r.second);
          return RuntimeResult().success(r.first);
        } else if (_node->token.get_name() == KEY_LT) {
          auto r = binaryLt(left.get_value(), right.get_value());
          if (r.second.has_error()) return RuntimeResult().failure(r.second);
          return RuntimeResult().success(r.first);
        } else if (_node->token.get_name() == KEY_GT) {
          auto r = binaryGt(left.get_value(), right.get_value());
          if (r.second.has_error()) return RuntimeResult().failure(r.second);
          return RuntimeResult().success(r.first);
        } else if (_node->token.get_name() == KEY_LE) {
          auto r = binaryLe(left.get_value(), right.get_value());
          if (r.second.has_error()) return RuntimeResult().failure(r.second);
          return RuntimeResult().success(r.first);
        } else if (_node->token.get_name() == KEY_GE) {
          auto r = binaryGe(left.get_value(), right.get_value());
          if (r.second.has_error()) return RuntimeResult().failure(r.second);
          return RuntimeResult().success(r.first);
        } else if (_node->token.get_name() == KEY_AND) {
          auto r = binaryAnd(left.get_value(), right.get_value());
          if (r.second.has_error()) return RuntimeResult().failure(r.second);
          return RuntimeResult().success(r.first);
        } else if (_node->token.get_name() == KEY_OR) {
          auto r = binaryOr(left.get_value(), right.get_value());
          if (r.second.has_error()) return RuntimeResult().failure(r.second);
          return RuntimeResult().success(r.first);
        }
        return RuntimeResult().failure(Error("Unknown Binary Operator", "Unknown Binary Operator Found"));
      }
      case node_type::_unary_operator: {
        auto right = visit(_node->right, scopeIndex, payload, __args);
        if (right.has_error()) return right;
        if (_node->token.get_name() == KEY_ADD) {
          return {RuntimeResult().success(right.get_value())};
        } else if (_node->token.get_name() == KEY_SUB) {
          auto r = unaryNeg(right.get_value());
          if (r.second.has_error()) return RuntimeResult().failure(r.second);
          return RuntimeResult().success(r.first);
        } else if (_node->token.get_name() == KEY_NOT) {
          auto r = unaryNot(right.get_value());
          if (r.second.has_error()) return RuntimeResult().failure(r.second);
          return RuntimeResult().success(r.first);
        }
        return RuntimeResult().failure(Error("Unknown Unary Operator", "Unknown Unary Operator Found"));
      }
      case node_type::_variable_assign: {
        if (payload != nullptr) {
          int _indx = -1;
          if (((*payload)->_data).find(_node->token.get_name()) == ((*payload)->_data).end()) {
            int itr = scopeIndex;
            while (itr >= 0 && getVarValue(scopes[itr], _node->token.get_name()).first == runtime_type::__ne) {
              itr--;
            }
            if (itr == -1)
              return RuntimeResult().failure(Error("Name Error", "Cannot declare object field outside object definition"));
            else
              _indx = itr;
          }
        }
        if (getVarValue(scopes[scopeIndex], _node->token.get_name()).first != runtime_type::__ne) {
          return RuntimeResult().failure(Error("Name Error", "Variable is already defined"));
        }

        auto value = visit(_node->left, scopeIndex, payload, __args);
        if (value.has_error()) return value;
        value.setConst(_node->isConst);
        if (value.get_value().first == __object) {
          value.setName(_node->token.get_name());
        }
        scopes[scopeIndex].localScope[_node->token.get_name()] = value.get_value();
        return RuntimeResult().success(value.get_value());
      }
      case node_type::_variable_reassign: {
        if (payload != nullptr) {
          int _indx = -1;
          if (((*payload)->_data).find(_node->token.get_name()) == ((*payload)->_data).end()) {
            int itr = scopeIndex;
            while (itr >= 0 && getVarValue(scopes[itr], _node->token.get_name()).first == runtime_type::__ne) {
              itr--;
            }
            if (itr == -1)
              return RuntimeResult().failure(Error("Name Error", "Field " + _node->token.get_name() + " is not defined for object"));
            else
              _indx = itr;
          }
          auto __r = visit(_node->left, scopeIndex, payload, __args);
          if (__r.has_error()) return __r;
          auto _res = __r.get_value();
          if (_res.first == __object) {
            ((obj *)_res.second)->_name = _node->token.get_name();
          }
          if (_indx != -1) {
            scopes[_indx].localScope[_node->token.get_name()] = _res;
            return RuntimeResult().success(_res);
          }
          ((*payload)->_data)[_node->token.get_name()] = _res;
          return RuntimeResult().success(_res);
        }
        int itr = scopeIndex;
        while (itr >= 0 && getVarValue(scopes[itr], _node->token.get_name()).first != runtime_type::__var) {
          itr--;
        }
        if (itr == -1) {
          return RuntimeResult().failure(Error("Name Error", "Unidentified token '" + _node->token.get_name() + "'"));
        }
        auto currValue = (data *)getVarValue(scopes[itr], _node->token.get_name()).second;
        if (currValue->isConst) {
          return RuntimeResult().failure(Error("Name Error", "Cannot reassign constant variable " + _node->token.get_name()));
        }
        auto value = visit(_node->left, scopeIndex, payload, __args);
        if (value.has_error()) return value;
        auto _res = value.get_value();
        scopes[itr].localScope[_node->token.get_name()] = _res;
        return RuntimeResult().success(_res);
      }
      case node_type::_variable_access: {
        if (payload != nullptr) {
          int _indx = -1;
          if (((*payload)->_data).find(_node->token.get_name()) == ((*payload)->_data).end()) {
            int itr = scopeIndex;
            while (itr >= 0 && scopes[itr].localScope.find(_node->token.get_name()) == scopes[itr].localScope.end()) {
              itr--;
            }
            if (itr == -1)
              return RuntimeResult().failure(Error("Name Error", "Field " + _node->token.get_name() + " is not defined for object"));
            else
              _indx = itr;
          }
          auto _res = _indx == -1 ? ((*payload)->_data)[_node->token.get_name()] : scopes[_indx].localScope[_node->token.get_name()];
          return RuntimeResult().success(_res);
        }
        // int _indx = isRegisteredObj(scopeIndex, _node->token.get_name());
        // if (_indx != -1) {
        //   return RuntimeResult().success(scopes[_indx].localScope[_node->token.get_name()]);
        // }
        int itr = scopeIndex;
        while (itr >= 0 && getVarValue(scopes[itr], _node->token.get_name()).first == runtime_type::__ne) {
          itr--;
        }
        if (itr == -1)
          return RuntimeResult().failure(Error("Name Error", "Variable '" + _node->token.get_name() + "' is not defined"));
        auto value = getVarValue(scopes[itr], _node->token.get_name());
        return RuntimeResult().success(value);
      }
      case node_type::_return: {
        if (!isFunction(scopeIndex)) {
          return RuntimeResult().failure(Error("Invalid Syntax Error", "'return' outside function block"));
        }
        auto _res = visit(_node->left, scopeIndex, payload, __args);
        _res.is_return = true;
        if (_res.has_error()) return {RuntimeResult().failure(_res.get_error())};
        if (_res.is_empty())
          return RuntimeResult().success({__var, new data(types::_null, type_value())});
        
        return RuntimeResult().success(_res.get_value());
      }
      case node_type::_function_block: {
        RuntimeResult result;
        for (auto child : _node->children) {
          if (child->value == node_type::_return) {
            auto _res = visit(child, scopeIndex, payload, __args);
            _res.is_return = true;
            return _res;
          }
          result = visit(child, scopeIndex, payload, __args);
          if (result.is_return) return result;
          if (result.has_error()) return result;
        }
        if (!result.is_empty()) return result;
        type_value v;
        v._int = 0;
        return RuntimeResult().success({__var, new data(types::_null, v)});
      }
      case node_type::_code_block: {
        scope new_scope;
        int scopeIndex = scopes.size();
        new_scope.index = scopeIndex;
        scopes.push_back(new_scope);
        RuntimeResult result;
        for (auto child : _node->children) {
          if (child->value == node_type::_return) {
            auto _res = visit(child, scopeIndex, payload, __args);
            _res.is_return = true;
            return _res;
          }
          result = visit(child, scopeIndex, payload, __args);
          if (result.is_return) return result;
          if (result.has_error()) return result;
        }
        scopes.erase(scopes.begin() + scopeIndex);
        if (isFunction(scopeIndex - 1)) {
          if (result.is_empty())
            return RuntimeResult().success({__var, new data(types::_null, type_value())});
        }
        return result;
      }
      case node_type::_if_else: {
        auto _cond = visit(_node->left, scopeIndex, payload, __args);
        if (_cond.has_error()) 
          return RuntimeResult().failure(_cond.get_error());
        data _res = type_cast(_cond.get_value(), _bool);
        if (_res.value._bool) {
          return visit(_node->right, scopeIndex, payload, __args);
        }
        for (auto elif : _node->children) {
          if (elif->token.get_name() == KEY_ELSE) return visit(elif->right, scopeIndex, payload, __args);
          _cond = visit(elif->left, scopeIndex, payload, __args);
          if (_cond.has_error()) 
            return RuntimeResult().failure(_cond.get_error());
          _res = type_cast(_cond.get_value(), _bool);
          if (_res.value._bool) {
            return visit(elif->right, scopeIndex, payload, __args);
          }
        }
        return RuntimeResult();
      }
      case node_type::_while_loop: {
        auto _cond = visit(_node->left, scopeIndex, payload, __args);
        data _res = type_cast(_cond.get_value(), _bool);
        while (_res.value._bool) {
          auto _r = visit(_node->right, scopeIndex, payload, __args);
          if (_r.has_error()) return _r;
          _cond = visit(_node->left, scopeIndex, payload, __args);
          _res = type_cast(_cond.get_value(), _bool);
        }
        return RuntimeResult();
      }
      case node_type::_function_definition: {
        bool _r = registerFunction(scopeIndex, _node->token.get_name(), &_node);
        if (!_r) return RuntimeResult().failure(Error("Name Error", "Function is already defined"));
        return RuntimeResult();
      }
      case node_type::_object_definition: {
        bool _r = registerObject(scopeIndex, _node->token.get_name(), &_node);
        if (!_r) return RuntimeResult().failure(Error("Name Error", "Object is already defined"));
        return RuntimeResult();
      }
      case node_type::_dot: {
        auto _res = visit(_node->left, scopeIndex, payload, __args);
        if (_res.has_error()) return _res;
        int _indx = -1;
        obj *_obj = (obj *)_res.get_value().second;
        bool isField = false;
        if (_obj == nullptr || isRegisteredObj(scopeIndex, _obj->_name) < 0) {
          if (payload != nullptr) {
            if ((*payload)->_data.find(_obj->_name) != (*payload)->_data.end()) {
              isField = true;
              if ((*payload)->_data.find(_obj->_name)->second.first != __object) {
                return RuntimeResult().failure(Error("Type Error", "Unknown object " + _node->left->token.get_name()));
              }
            }
          }
          if (!isField)
            return RuntimeResult().failure(Error("Name Error", "Unknown object " + _node->left->token.get_name()));
        }
        if (!isField) {
          _indx = isRegisteredObj(scopeIndex, _obj->_name);
          if (_indx == -1) return RuntimeResult().failure(Error("Name Error", "Object is not defined"));
        }
        auto obj_def = _obj->_def;
        if (_node->right->value == node_type::_function_call) {
          for (auto child : obj_def->children) {
            if (child->value == node_type::_method_call) {
              if (child->token.get_name() == _node->right->token.get_name()) {
                auto _args = _node->right->children;
                auto _res = visit(child, scopeIndex, &_obj, _args);
                if (_res.has_error()) return _res;
                return RuntimeResult().success(_res.get_value());
              }
            }
          }
          return RuntimeResult().failure(Error("Name Error", "Method is not defined"));
        }
        if (_obj->_data.find(_node->right->token.get_name()) == _obj->_data.end()) {
          return RuntimeResult().failure(Error("Name Error", "Unknown property " + _node->right->token.get_name()));
        }
        return RuntimeResult().success(_obj->_data[_node->right->token.get_name()]);
      }
      case node_type::_function_call: {
        int _indx = isRegisteredFunc(scopeIndex, _node->token.get_name());
        if (_indx == -1) {
          if (isPrimitive(_node->token.get_name())) {
            auto _res = handlePrimitive(_node, scopeIndex, payload);
            if (_res.second.has_error()) {
              return RuntimeResult().failure(_res.second);
            }
            return RuntimeResult().success(_res.first);
          }
          _indx = isDefinedObj(scopeIndex, _node->token.get_name());
          if (_indx != -1) {
            auto _res = instantiateObject(scopeIndex, _node, payload, __args);
            if (_res.second.has_error()) return RuntimeResult().failure(_res.second);
            return RuntimeResult().success({__object, _res.first});
          }
          if (payload == nullptr)
            return RuntimeResult().failure(Error("Name Error", "Function/Object is not defined"));
          else {
            scope new_scope;
            int scopeIndex = scopes.size();
            new_scope.index = scopeIndex;
            new_scope.function = true;
            scopes.push_back(new_scope);
            // for(auto itr : payload->_data) {
            //   scopes[scopeIndex].localScope[itr.first] = itr.second;
            // }
            auto _res = visit(_node->left, scopeIndex, payload, __args);
            scopes.erase(scopes.begin() + scopeIndex);
            if (_res.has_error()) return _res;
            return _res;
            return RuntimeResult().failure(Error("Name Error", "here!"));
          }
        }
        auto __res = scopes[_indx].localScope[_node->token.get_name()];
        if (__res.first != __func) 
          return RuntimeResult().failure(Error("Name Error", "Function is not defined"));
        node *_func = (node *)__res.second;
        if (_func->args.size() != _node->children.size()) {
          return RuntimeResult().failure(Error("Name Error", "Function arguments are not matching"));
        }
        scope new_scope;
        int scopeIndex = scopes.size();
        new_scope.index = scopeIndex;
        new_scope.function = true;
        scopes.push_back(new_scope);
        for(int i = 0; i < _func->args.size(); i++) {
          auto _res = visit(_node->children[i], scopeIndex, payload, __args);
          if (_res.has_error()) return _res;
          scopes[scopeIndex].localScope[_func->args[i].get_name()] = _res.get_value();
        }
        auto _res = visit(_func->left, scopeIndex, payload, __args);
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