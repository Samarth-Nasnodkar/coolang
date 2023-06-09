#include "../variables/data.h"

#ifndef TYPE_CASTING_H
#define TYPE_CASTING_H

data type_cast(__runtime_var _o, types _type_to) {
  if (_o.first != __var) {
    auto _out = type_value();
    _out._bool = false;
    return data(_bool, _out);
  }
  data a = *((data *)_o.second);
  if (a._type == _type_to) {
    return a;
  }
  type_value out;
  if (_type_to == _int) {
    if (a._type == _float) {
      out._int = (int)a.value._float;
    } else {
      out = a.value;
    }
    return data(_int, out);
  } else if (_type_to == _float) {
    if (a._type == _int) {
      out._float = (float)a.value._int;
    } else {
      out = a.value;
    }
    return data(_float, out);
  } else if (_type_to == _bool) {
    if (a._type == _int) {
      out._bool = (bool)a.value._int;
    } else if (a._type == _float) {
      out._bool = (bool)a.value._float;
    } else {
      out = a.value;
    }
    return data(_bool, out);
  }
  return data();
}

#endif