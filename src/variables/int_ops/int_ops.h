#include "../../includes.h"
#include "../operators.h"
#include "../data.h"
#include "../../error/error.h"

#ifndef INT_OPS_H
#define INT_OPS_H

std::pair<data, Error> int_add(data a, data b) {
  type_value _res;
  if (b._type == types::_int) {
    _res._int = a.value._int + b.value._int;
  } else if (b._type == types::_float) {
    _res._int = a.value._int + b.value._float;
  } else if (b._type == types::_string) {
    return {data(), Error("Invalid Operator", "Cannot add str to int")};
  } else if (b._type == types::_string) {
    return {data(), Error("Invalid Operator", "Cannot add null to int")};
  }
  return {data(types::_int, _res), Error()};
}

std::pair<data, Error> int_sub(data a, data b) {
  type_value _res;
  if (b._type == types::_int) {
    _res._int = a.value._int - b.value._int;
  } else if (b._type == types::_float) {
    _res._int = a.value._int - b.value._float;
  } else if (b._type == types::_string) {
    return {data(), Error("Invalid Operator", "Cannot subtract str from int")};
  } else if (b._type == types::_null) {
    return {data(), Error("Invalid Operator", "Cannot subtract null from int")};
  }
  return {data(types::_int, _res), Error()};
}

std::pair<data, Error> int_mul(data a, data b) {
  type_value _res;
  if (b._type == types::_int) {
    _res._int = a.value._int * b.value._int;
  } else if (b._type == types::_float) {
    _res._int = a.value._int * b.value._float;
  } else if (b._type == types::_string) {
    int c = a.value._int;
    int b_len = strnlen(b.value._string, 10000);
    _res._string = new char[b_len * c + 1];
    for(int i = 0; i < c; i++)
      strncat(_res._string, b.value._string, b_len);
    
    return {data(types::_string, _res), Error()};
  } else if (b._type == types::_null) {
    return {data(), Error("Invalid Operator", "Cannot multiply int by null")};
  }
  return {data(types::_int, _res), Error()};
}

std::pair<data, Error> int_div(data a, data b) {
  type_value _res;
  if (b._type == types::_int) {
    if (b.value._int == 0) {
      return {data(), Error("Zero Division Error", "Cannot divide int by zero")};
    }
    _res._int = a.value._int / b.value._int;
  } else if (b._type == types::_float) {
    if (b.value._float == 0) {
      return {data(), Error("Zero Division Error", "Cannot divide int by zero")};
    }
    _res._int = a.value._int / b.value._float;
  } else if (b._type == types::_string) {
    return {data(), Error("Invalid Operator", "Cannot divide int by str")};
  } else if (b._type == types::_null) {
    return {data(), Error("Invalid Operator", "Cannot divide int by null")};
  }
  return {data(types::_int, _res), Error()};
}

std::pair<data, Error> int_mod(data a, data b) {
  type_value _res;
  if (b._type == types::_int) {
    if (b.value._float == 0) {
      return {data(), Error("Zero Division Error", "Cannot modulo int by zero")};
    }
    _res._int = a.value._int % b.value._int;
  } else if (b._type == types::_float) {
    return {data(), Error("Invalid Operator", "Cannot find modulo of int by float")};
  } else if (b._type == types::_string) {
    return {data(), Error("Invalid Operator", "Cannot find modulo of int by str")};
  } else if (b._type == types::_null) {
    return {data(), Error("Invalid Operator", "Cannot find modulo of int by null")};
  }
  return {data(types::_int, _res), Error()};
}

std::pair<data, Error> int_pow(data a, data b) {
  type_value _res;
  if (b._type == types::_int) {
    _res._int = pow(a.value._int, b.value._int);
  } else if (b._type == types::_float) {
    _res._float = pow(a.value._int, b.value._int);
    return {data(types::_float, _res), Error()};
  } else if (b._type == types::_string) {
    return {data(), Error("Invalid Operator", "Cannot raise int to exp of type str")};
  } else if (b._type == types::_null) {
    return {data(), Error("Invalid Operator", "Cannot raise int to exp of type null")};
  }
  return {data(types::_int, _res), Error()};
}

std::pair<data, Error> int_cmp(data a, data b) {
  type_value _res;
  if (b._type == types::_int) {
    _res._int = a.value._int - b.value._int;
  } else if (b._type == types::_float) {
    _res._int = a.value._int - b.value._float;
  } else if (b._type == types::_string) {
    return {data(), Error("Invalid Operator", "Cannot compare int and str")};
  } else if (b._type == types::_null) {
    return {data(), Error("Invalid Operator", "Cannot compare int and null")};
  }
  return {data(types::_int, _res), Error()};
}

#endif