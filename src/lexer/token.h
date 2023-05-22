#include "../includes.h"
#include "../variables/data.h"

#ifndef TOKEN_H
#define TOKEN_H

class Token {
  std::string name;
  CursorPosition start;
  CursorPosition end;
  data value;
  bool _hasData;
public:
  Token() {
    _hasData = false;
  }

  Token(std::string _name, data _value, CursorPosition _start, CursorPosition _end) {
    name = _name;
    value = _value;
    _hasData = true;
    start = _start;
    end = _end;
  }

  Token(std::string _name, CursorPosition _start, CursorPosition _end) {
    name = _name;
    _hasData = false;
    start = _start;
    end = _end;
  }

  std::string get_name() {
    return name;
  }

  data get_value() {
    if (_hasData)
      return value;

    return data();
  }

  bool has_data() {
    return _hasData;
  }

  std::string to_string() {
    std::string _repr;
    _repr += "(" + name;
    if (has_data()) {
      _repr += ": " + value.to_string() + ")";
    } else {
      _repr += ")";
    }
    return _repr;
  }
};

#endif