#include "../variables/data.h"
#include "../error/error.h"
#include "scope.h"

#ifndef RUNTIME_RESULT_H
#define RUNTIME_RESULT_H

class RuntimeResult {
  std::vector<data> values;
  obj *obj_value;
  data value;
  Error error;
  bool empty;
  bool list;
public:
  bool is_return;
  RuntimeResult() {
    obj_value = nullptr;
    value = data();
    error = Error();
    empty = true;
    list = false;
    is_return = false;
  }

  void reset() {
    obj_value = nullptr;
    value = data();
    error = Error();
    empty = true;
    list = false;
  }

  data get_value() {
    return value;
  }

  bool isConst() {
    return value.isConst;
  }

  void setConst(bool _const) {
    value.isConst = _const;
  }

  void set_value(data _value) {
    empty = false;
    value = _value;
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

  obj **get_obj() {
    return &obj_value;
  }

  void setName(std::string _name) {
    obj_value->_name = _name;
  }

  std::string to_string() {
    if (error.has_error()) {
      return error.to_string();
    }
    return value.to_string();
  }

  RuntimeResult success(data _value) {
    obj_value = nullptr;
    value = _value;
    empty = false;
    return *this;
  }

  RuntimeResult success(obj **_obj) {
    obj_value = *_obj;
    empty = false;
    return *this;
  }

  RuntimeResult success(std::vector<data> &_values) {
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