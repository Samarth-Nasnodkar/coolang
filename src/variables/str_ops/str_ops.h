#include "../../includes.h"
#include "../operators.h"
#include "../data.h"
#include "../../error/error.h"

#ifndef STR_OPS_H
#define STR_OPS_H

std::pair<data, Error> str_add(data a, data b) {
  type_value _res;
  if (b._type == types::_int || b._type == types::_float || b._type == types::_string) {
    std::string r = b.to_string();
    int a_len = strnlen(a.value._string, 10000);
    int b_len = strnlen(b.value._string, 10000);
    _res._string = new char[a_len + b_len + 1];
    strncpy(_res._string, a.value._string, a_len);
    strncat(_res._string, b.value._string, b_len);
    return {data(types::_string, _res), Error()};
  } else if (b._type == types::_null) {
    return {data(), Error("Invalid Operator", "Cannot add str and null")};
  }
  return {data(types::_float, _res), Error()};
}

std::pair<data, Error> str_sub(data a, data b) {
  return {data(), Error("Invalid Operator", "Cannot subtract from str")};
}

std::pair<data, Error> str_mul(data a, data b) {
  type_value _res;
  if (b._type == types::_int) {
    int c = b.value._int;
    int a_len = strnlen(a.value._string, 10000);
    _res._string = new char[a_len * c + 1];
    for(int i = 0; i < c; i++)
      strncat(_res._string, a.value._string, a_len);
    
    return {data(types::_string, _res), Error()};
  } else if (b._type == types::_float) {
    return {data(), Error("Invalid Operator", "Cannot multiply str and float")};
  } else if (b._type == types::_string) {
    return {data(), Error("Invalid Operator", "Cannot multiply str and str")};
  } else if (b._type == types::_null) {
    return {data(), Error("Invalid Operator", "Cannot multiply str and null")};
  }
  return {data(types::_string, _res), Error()};
}

std::pair<data, Error> str_div(data a, data b) {
  return {data(), Error("Invalid Operator", "Cannot divide from str")};
}

std::pair<data, Error> str_mod(data a, data b) {
  return {data(), Error("Invalid Operator", "Cannot modulo str")};
}

std::pair<data, Error> str_pow(data a, data b) {
  return {data(), Error("Invalid Operator", "Cannot raise str to exp")};
}

std::pair<data, Error> str_cmp(data a, data b) {
  type_value _res;
  if (b._type == types::_int) {
    return {data(), Error("Invalid Operator", "Cannot compare str and int")};
  } else if (b._type == types::_float) {
    return {data(), Error("Invalid Operator", "Cannot compare str and float")};
  } else if (b._type == types::_string) {
    _res._int = strncmp(a.value._string, b.value._string, std::max(strnlen(a.value._string, 10000), strnlen(b.value._string, 10000)));
  } else if (b._type == types::_null) {
    return {data(), Error("Invalid Operator", "Cannot compare str and null")};
  }
  return {data(types::_int, _res), Error()};
}

#endif