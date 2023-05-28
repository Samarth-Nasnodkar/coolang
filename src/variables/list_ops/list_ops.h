#include "../../includes.h"
#include "../operators.h"
#include "../data.h"
#include "../../error/error.h"

#ifndef LIST_OPS_H
#define LIST_OPS_H

std::pair<data, Error> list_add(data a, data b) {
  type_value _res;
  if (b._type == types::_list_type) {
    data *_data = new data[a.value._list->size + b.value._list->size];
    memcpy(_data, a.value._list->_data, a.value._list->size * sizeof(data));
    memcpy(_data + a.value._list->size, b.value._list->_data, b.value._list->size * sizeof(data));
    _res._list = new _llist{_data, a.value._list->size + b.value._list->size};
  } else {
    data *_data = new data[a.value._list->size + 1];
    memcpy(_data, a.value._list->_data, a.value._list->size * sizeof(data));
    _data[a.value._list->size] = b;
    _res._list = new _llist{_data, a.value._list->size + 1};
  }
  return {data(types::_list_type, _res), Error()};
}

std::pair<data, Error> list_sub(data a, data b) {
  return {data(), Error("Invalid Operator", "Cannot subtract from list")};
}

std::pair<data, Error> list_mul(data a, data b) {
  type_value _res;
  if (b._type == types::_int) {
    int a_len = a.value._list->size;
    int final_len = a_len * b.value._int;
    data *_data = new data[final_len];
    for(int i = 0; i < b.value._int; ++i)
    memcpy(_data + (i * a_len), a.value._list->_data, a_len * sizeof(data));
    _res._list = new _llist{_data, final_len};
  } else if (b._type == types::_float) {
    return {data(), Error("Invalid Operator", "Cannot multiply list by float")};
  } else if (b._type == types::_string) {
    return {data(), Error("Invalid Operator", "Cannot multiply list by str")};
  } else if (b._type == types::_null) {
    return {data(), Error("Invalid Operator", "Cannot multiply list by null")};
  }
  return {data(types::_list_type, _res), Error()};
}

std::pair<data, Error> list_div(data a, data b) {
  return {data(), Error("Invalid Operator", "Cannot divide list")};
}

std::pair<data, Error> list_mod(data a, data b) {
  return {data(), Error("Invalid Operator", "Cannot modulo list")};
}

std::pair<data, Error> list_pow(data a, data b) {
  return {data(), Error("Invalid Operator", "Cannot raise list to exp")};
}

std::pair<data, Error> list_cmp(data a, data b) {
  type_value _res;
  if (b._type == types::_int) {
    return {data(), Error("Invalid Operator", "Cannot compare list and int")};
  } else if (b._type == types::_float) {
    return {data(), Error("Invalid Operator", "Cannot compare list and float")};
  } else if (b._type == types::_string) {
    return {data(), Error("Invalid Operator", "Cannot compare list and str")};
  } else if (b._type == types::_null) {
    return {data(), Error("Invalid Operator", "Cannot compare list and null")};
  } else if (b._type == types::_list_type) {
    return {data(), Error("Invalid Operator", "Cannot compare list and list")};
  }
  return {data(types::_int, _res), Error()};
}

#endif