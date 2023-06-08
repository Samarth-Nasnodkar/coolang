#include "../variables/data.h"
#include "../error/error.h"
#include "scope.h"

#ifndef RUNTIME_RESULT_H
#define RUNTIME_RESULT_H

class RuntimeResult {
  std::vector<std::map<runtime_type, void *>> values;
  obj *obj_value;
  data *var_value;
  node *func_value;
  runtime_type _type;
  Error error;
  bool empty;
  bool list;
public:
  bool is_return;
  RuntimeResult() {
    reset();
  }

  void reset() {
    obj_value = nullptr;
    var_value = nullptr;
    func_value = nullptr;
    error = Error();
    empty = true;
    list = false;
    is_return = false;
  }

  __runtime_var get_value() {
    if (var_value != nullptr)
      return {__var, var_value};
    else if (obj_value != nullptr)
      return {__object, obj_value};
    else if (func_value != nullptr)
      return {__func, func_value};
    else
      return {__ne, nullptr};
  }

  bool isConst() {
    return var_value->isConst;
  }

  void setConst(bool _const) {
    return;
    var_value->isConst = _const;
  }

  void set_value(data *_value) {
    reset();
    empty = false;
    var_value = _value;
  }

  bool is_empty() {
    return empty;
  }

  Error get_error() {
    return error;
  }

  void set_error(Error _error) {
    empty = false;
    error = _error;
  }

  bool has_error() {
    return error.has_error();
  }

  obj *get_obj() {
    return obj_value;
  }

  void setName(std::string _name) {
    obj_value->_name = _name;
  }

  std::string to_string() {
    if (error.has_error()) {
      return error.to_string();
    }
    return var_value->to_string();
  }

  RuntimeResult success(__runtime_var _value) {
    if (_value.first == __object) {
      obj_value = (obj *)_value.second;
      empty = false;
    } else if (_value.first == __var) {
      var_value = (data *)_value.second;
      empty = false;
    } else if (_value.first == __func) {
      func_value = (node *)_value.second;
      empty = false;
    } else {
      empty = true;
    }
    return *this;
  }

  RuntimeResult success(std::vector<std::map<runtime_type, void *>> &_values) {
    obj_value = nullptr;
    values = _values;
    empty = false;
    list = true;
    return *this;
  }

  RuntimeResult failure(Error _error) {
    error = _error;
    empty = false;
    return *this;
  }
};

#endif // RUNTIME_RESULT_H