#include "types.h"
#include "type_value.h"

#ifndef DATA_H
#define DATA_H

struct  data {
  types _type;
  type_value value;

  data() {
    _type = types::_null;
    value = type_value();
  }

  data(types __type, type_value _value) {
    _type = __type;
    value = _value;
  }

  std::string to_string() {
    std::string _repr;
    if (_type == types::_int)
      _repr += std::to_string(value._int);
    else if (_type == types::_float)
      _repr += std::to_string(value._float);
    else if (_type == types::_bool) {
      _repr += value._bool ? "true" : "false";
    }
    else if (_type == types::_string) {
      _repr += value._string;
    }
    else if (_type == types::_list_type) {
      if (value._list->size == 0)
        return _repr += "[]";
      
      _repr += "[";
      data *_data = (data*)value._list->_data;
      for (int i = 0; i < value._list->size; i++) {
        _repr += _data[i].to_string();
        if (i != value._list->size - 1)
          _repr += ", ";
      }
      _repr += "]";
    }
    else
      _repr += "null";
    
    return _repr;
  }
};

#endif // DATA_H