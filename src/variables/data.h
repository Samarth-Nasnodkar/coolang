#include "types.h"
#include "type_value.h"
#include "../interpreter/runtime_type.h"

#ifndef DATA_H
#define DATA_H

struct data;

std::string data_to_str(data _d);

struct data {
  types _type;
  bool isConst;
  type_value value;

  data() {
    isConst = false;
    _type = types::_null;
    value = type_value();
  }

  data(types __type, type_value _value) {
    isConst = false;
    _type = __type;
    value = _value;
  }

  std::string to_string() {
    return data_to_str(*this);
  }
};

std::string data_to_str(data _d) {
  std::string _repr;
  if (_d._type == types::_int)
    _repr += std::to_string(_d.value._int);
  else if (_d._type == types::_float)
    _repr += std::to_string(_d.value._float);
  else if (_d._type == types::_bool) {
    _repr += _d.value._bool ? "true" : "false";
  }
  else if (_d._type == types::_string) {
    _repr += _d.value._string;
  }
  else if (_d._type == types::_list_type) {
    if (_d.value._list->size == 0)
      return _repr += "[]";
    
    _repr += "[";
    __runtime_var *_data = (__runtime_var*)_d.value._list->_data;
    for (int i = 0; i < _d.value._list->size; i++) {
      if (_data[i].first == __var) {
        _repr += ((data *)_data[i].second)->to_string();
      } else if (_data[i].first == __object) {
        std::stringstream ss;
        ss << _data[i].second;
        _repr += "<Object at " + ss.str() + ">";
      } else if (_data[i].first == __func) {
        std::stringstream ss;
        ss << _data[i].second;
        _repr += "<Function at " + ss.str() + ">";
      } else if (_data[i].first == __ne) {
        _repr += "undefined";
      }
      else
        _repr += "null";
      if (i != _d.value._list->size - 1)
        _repr += ", ";
    }
    _repr += "]";
  }
  else
    _repr += "null";
  
  return _repr;
}

#endif // DATA_H