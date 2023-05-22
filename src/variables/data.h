#include "types.h"
#include "type_value.h"

#ifndef DATA_H
#define DATA_H

struct data {
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
};

#endif // DATA_H