#include "../includes.h"
#include "../variables/data.h"

#ifndef PRIMITIVE_FUNC_H
#define PRIMITIVE_FUNC_H

data print(data *a, int n) {
  for(int i = 0; i < n; ++i)
    std::cout << a[i].to_string() << " ";
  std::cout << std::endl;
  return data();
}

data len(data a) {
  if (a._type == types::_string) {
    int _len = strlen(a.value._string);
    type_value v;
    v._int = _len;
    return data(types::_int, v);
  } else if (a._type == types::_list_type) {
    int _len = a.value._list->size;
    type_value v;
    v._int = _len;
    return data(types::_int, v);
  } else {
    std::cout << "TypeError: len() only accepts strings and lists" << std::endl;
    exit(1);
  }
}

data type_of(data a) {
  type_value v;
  std::string _type_str = types_str[a._type];
  v._string = new char[_type_str.length() + 1];
  strncpy(v._string, _type_str.c_str(), _type_str.length());
  return data(types::_string, v);
}

std::string primitive_funcs[] = {
  "print",
  "type_of",
  "len",
};

#endif