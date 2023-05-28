#include "../../includes.h"
#include "../operators.h"
#include "../data.h"
#include "../../error/error.h"

#ifndef FLOAT_OPS_H
#define FLOAT_OPS_H

std::pair<data, Error> float_add(data a, data b) {
  type_value _res;
  if (b._type == types::_int) {
    _res._float = a.value._float + b.value._int;
  } else if (b._type == types::_float) {
    _res._float = a.value._float + b.value._float;
  } else if (b._type == types::_string) {
    return {data(), Error("Invalid Operator", "Cannot add str to float")};
  } else if (b._type == types::_null) {
    return {data(), Error("Invalid Operator", "Cannot add null to float")};
  }
  return {data(types::_float, _res), Error()};
}

std::pair<data, Error> float_sub(data a, data b) {
  type_value _res;
  if (b._type == types::_int) {
    _res._float = a.value._float - b.value._int;
  } else if (b._type == types::_float) {
    _res._float = a.value._float - b.value._float;
  } else if (b._type == types::_string) {
    return {data(), Error("Invalid Operator", "Cannot subtract str from float")};
  } else if (b._type == types::_null) {
    return {data(), Error("Invalid Operator", "Cannot subtract null from float")};
  }
  return {data(types::_float, _res), Error()};
}

std::pair<data, Error> float_mul(data a, data b) {
  type_value _res;
  if (b._type == types::_int) {
    _res._float = a.value._float * b.value._int;
  } else if (b._type == types::_float) {
    _res._float = a.value._float * b.value._float;
  } else if (b._type == types::_string) {
    return {data(), Error("Invalid Operator", "Cannot multiply float and str")};
  } else if (b._type == types::_null) {
    return {data(), Error("Invalid Operator", "Cannot multiply float by null")};
  }
  return {data(types::_float, _res), Error()};
}

std::pair<data, Error> float_div(data a, data b) {
  type_value _res;
  if (b._type == types::_int) {
    if (b.value._int == 0) {
      return {data(), Error("Zero Division Error", "Cannot divide float by zero")};
    }
    _res._float = a.value._float / b.value._int;
  } else if (b._type == types::_float) {
    if (b.value._float == 0) {
      return {data(), Error("Zero Division Error", "Cannot divide float by zero")};
    }
    _res._float = a.value._float / b.value._float;
  } else if (b._type == types::_string) {
    return {data(), Error("Invalid Operator", "Cannot divide float by str")};
  } else if (b._type == types::_null) {
    return {data(), Error("Invalid Operator", "Cannot divide float by null")};
  }
  return {data(types::_float, _res), Error()};
}

std::pair<data, Error> float_mod(data a, data b) {
  type_value _res;
  if (b._type == types::_int) {
    return {data(), Error("Invalid Operator", "Cannot find modulo of float by float")};
  } else if (b._type == types::_float) {
    return {data(), Error("Invalid Operator", "Cannot find modulo of float by float")};
  } else if (b._type == types::_string) {
    return {data(), Error("Invalid Operator", "Cannot find modulo of float by str")};
  } else if (b._type == types::_null) {
    return {data(), Error("Invalid Operator", "Cannot find modulo of float by null")};
  }
  return {data(types::_float, _res), Error()};
}

std::pair<data, Error> float_pow(data a, data b) {
  type_value _res;
  if (b._type == types::_int) {
    _res._float = pow(a.value._float, b.value._int);
  } else if (b._type == types::_float) {
    _res._float = pow(a.value._float, b.value._int);
    return {data(types::_float, _res), Error()};
  } else if (b._type == types::_string) {
    return {data(), Error("Invalid Operator", "Cannot raise float to exp of type str")};
  } else if (b._type == types::_null) {
    return {data(), Error("Invalid Operator", "Cannot raise float to exp of type null")};
  }
  return {data(types::_float, _res), Error()};
}

std::pair<data, Error> float_cmp(data a, data b) {
  type_value _res;
  if (b._type == types::_int) {
    _res._int = a.value._float - b.value._int;
  } else if (b._type == types::_float) {
    _res._int = a.value._float - b.value._float;
  } else if (b._type == types::_string) {
    return {data(), Error("Invalid Operator", "Cannot compare int and str")};
  } else if (b._type == types::_string) {
    return {data(), Error("Invalid Operator", "Cannot compare float and null")};
  } 
  return {data(types::_int, _res), Error()};
}

#endif