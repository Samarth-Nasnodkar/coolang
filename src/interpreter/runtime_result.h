#include "../variables/data.h"
#include "../error/error.h"

#ifndef RUNTIME_RESULT_H
#define RUNTIME_RESULT_H

class RuntimeResult {
  data value;
  Error error;
  bool empty;
public:
  RuntimeResult() {
    value = data();
    error = Error();
    empty = true;
  }

  void reset() {
    value = data();
    error = Error();
    empty = true;
  }

  data get_value() {
    return value;
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

  std::string to_string() {
    if (error.has_error()) {
      return error.to_string();
    }
    return value.to_string();
  }

  RuntimeResult success(data _value) {
    value = _value;
    empty = false;
    return *this;
  }

  RuntimeResult failure(Error _error) {
    error = _error;
    empty = false;
    return *this;
  }
};

#endif // RUNTIME_RESULT_H