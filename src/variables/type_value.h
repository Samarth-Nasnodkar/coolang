#include "../includes.h"

#ifndef TYPE_VALUE_H
#define TYPE_VALUE_H

union type_value {
  int _int;
  float _float;
  char _char;
  bool _bool;
  char *_string;

  type_value() {
    _int = 0;
  }
};

#endif // TYPE_VALUE_H
