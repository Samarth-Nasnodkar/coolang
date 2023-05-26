#include "operators.h"
#include "data.h"

#ifndef OPERATIONS_H
#define OPERATIONS_H

bool asInt(data a) {
  return a._type == _int || a._type == _bool || a._type == _null;
}

types getType(data a, data b) {
  if (a._type == b._type)
    return a._type;
  else if (a._type == types::_float || b._type == types::_float)
    return types::_float;
  else
    return a._type != types::_null ? a._type : b._type;
}

data add(data a, data b) {
  type_value res;
  if (asInt(a) && asInt(b)) {
    res._int = a.value._int + b.value._int;
    return data(getType(a, b), res);
  } else if (asInt(a) && b._type == types::_float) {
    res._float = a.value._int + b.value._float;
    return data(getType(a, b), res);
  } else if (a._type == types::_float && asInt(b)) {
    res._float = a.value._float + b.value._int;
    return data(getType(a, b), res);
  } else if (a._type == types::_float && b._type == types::_float) {
    res._float = a.value._float + b.value._float;
    return data(getType(a, b), res);
  } else {
    return data();
  }
}

data sub(data a, data b) {
  type_value res;
  if (asInt(a) && asInt(b)) {
    res._int = a.value._int - b.value._int;
    return data(getType(a, b), res);
  } else if (asInt(a) && b._type == types::_float) {
    res._float = a.value._int - b.value._float;
    return data(getType(a, b), res);
  } else if (a._type == types::_float && asInt(b)) {
    res._float = a.value._float - b.value._int;
    return data(getType(a, b), res);
  } else if (a._type == types::_float && b._type == types::_float) {
    res._float = a.value._float - b.value._float;
    return data(getType(a, b), res);
  } else {
    return data();
  }
}

data mul(data a, data b) {
  type_value res;
  if (asInt(a) && asInt(b)) {
    res._int = a.value._int * b.value._int;
    return data(getType(a, b), res);
  } else if (asInt(a) && b._type == types::_float) {
    res._float = a.value._int * b.value._float;
    return data(getType(a, b), res);
  } else if (a._type == types::_float && asInt(b)) {
    res._float = a.value._float * b.value._int;
    return data(getType(a, b), res);
  } else if (a._type == types::_float && b._type == types::_float) {
    res._float = a.value._float * b.value._float;
    return data(getType(a, b), res);
  } else {
    return data();
  }
}

data div(data a, data b) {
  type_value res;
  if (asInt(a) && asInt(b)) {
    res._int = a.value._int / b.value._int;
    return data(getType(a, b), res);
  } else if (asInt(a) && b._type == types::_float) {
    res._float = a.value._int / b.value._float;
    return data(getType(a, b), res);
  } else if (a._type == types::_float && asInt(b)) {
    res._float = a.value._float / b.value._int;
    return data(getType(a, b), res);
  } else if (a._type == types::_float && b._type == types::_float) {
    res._float = a.value._float / b.value._float;
    return data(getType(a, b), res);
  } else {
    return data();
  }
}

data mod(data a, data b) {
  type_value res;
  if (asInt(a) && asInt(b)) {
    res._int = a.value._int % b.value._int;
    return data(getType(a, b), res);
  } else if (asInt(a) && b._type == types::_float) {
    res._float = fmod(a.value._int, b.value._float);
    return data(getType(a, b), res);
  } else if (a._type == types::_float && asInt(b)) {
    res._float = fmod(a.value._float, b.value._int);
    return data(getType(a, b), res);
  } else if (a._type == types::_float && b._type == types::_float) {
    res._float = fmod(a.value._float, b.value._float);
    return data(getType(a, b), res);
  } else {
    return data();
  }
}

data pow(data a, data b) {
  type_value res;
  if (asInt(a) && asInt(b)) {
    res._int = pow(a.value._int, b.value._int);
    return data(getType(a, b), res);
  } else if (asInt(a) && b._type == types::_float) {
    res._float = pow(a.value._int, b.value._float);
    return data(getType(a, b), res);
  } else if (a._type == types::_float && asInt(b)) {
    res._float = pow(a.value._float, b.value._int);
    return data(getType(a, b), res);
  } else if (a._type == types::_float && b._type == types::_float) {
    res._float = pow(a.value._float, b.value._float);
    return data(getType(a, b), res);
  } else {
    return data();
  }
}

data neg(data a) {
  type_value res;
  if (asInt(a)) {
    res._int = -a.value._int;
    return data(types::_int, res);
  } else if (a._type == types::_float) {
    res._float = -a.value._float;
    return data(types::_float, res);
  } else {
    return data();
  }
}

data logic_not(data a) {
  type_value res;
  // if (asInt(a)) {
  //   res._bool = !a.value._bool;
  //   return {types::_bool, res};
  // } else if (a._type == types::_float) {
  //   res._bool = !a.value._float;
  //   return {types::_bool, res};
  // }
  res._bool = !a.value._int;
  return {types::_bool, res};
}

data logic_and(data a, data b) {
  type_value res;
  // if (asInt(a) && asInt(b)) {
  //   res._bool = a.value._int && b.value._int;
  //   return data(types::_bool, res);
  // } else if (asInt(a) && b._type == types::_float) {
  //   res._bool = a.value._int && b.value._float;
  //   return data(types::_bool, res);
  // } else if (a._type == types::_float && asInt(b)) {
  //   res._bool = a.value._float && b.value._int;
  //   return data(types::_bool, res);
  // } else if (a._type == types::_float && b._type == types::_float) {
  //   res._bool = a.value._float && b.value._float;
  //   return data(types::_bool, res);
  // } else {
  //   return data();
  // }
  res._bool = a.value._int && b.value._int;
  return data(types::_bool, res);
}

data logic_or(data a, data b) {
  type_value res;
  // if (asInt(a) && asInt(b)) {
  //   res._bool = a.value._int || b.value._int;
  //   return data(types::_bool, res);
  // } else if (asInt(a) && b._type == types::_float) {
  //   res._bool = a.value._int || b.value._float;
  //   return data(types::_bool, res);
  // } else if (a._type == types::_float && asInt(b)) {
  //   res._bool = a.value._float || b.value._int;
  //   return data(types::_bool, res);
  // } else if (a._type == types::_float && b._type == types::_float) {
  //   res._bool = a.value._float || b.value._float;
  //   return data(types::_bool, res);
  // } else {
  //   return data();
  // }
  res._bool = a.value._int || b.value._int;
  return data(types::_bool, res);
}

data le(data a, data b) {
  type_value res;
  if (asInt(a) && asInt(b)) {
    res._bool = a.value._int <= b.value._int;
    return data(types::_bool, res);
  } else if (asInt(a) && b._type == types::_float) {
    res._bool = a.value._int <= b.value._float;
    return data(types::_bool, res);
  } else if (a._type == types::_float && asInt(b)) {
    res._bool = a.value._float <= b.value._int;
    return data(types::_bool, res);
  } else if (a._type == types::_float && b._type == types::_float) {
    res._bool = a.value._float <= b.value._float;
    return data(types::_bool, res);
  } else {
    return data();
  }
}

data lt(data a, data b) {
  type_value res;
  if (asInt(a) && asInt(b)) {
    res._bool = a.value._int < b.value._int;
    return data(types::_bool, res);
  } else if (asInt(a) && b._type == types::_float) {
    res._bool = a.value._int < b.value._float;
    return data(types::_bool, res);
  } else if (a._type == types::_float && asInt(b)) {
    res._bool = a.value._float < b.value._int;
    return data(types::_bool, res);
  } else if (a._type == types::_float && b._type == types::_float) {
    res._bool = a.value._float < b.value._float;
    return data(types::_bool, res);
  } else {
    return data();
  }
}

data ge(data a, data b) {
  type_value res;
  if (asInt(a) && asInt(b)) {
    res._bool = a.value._int > b.value._int;
    return data(types::_bool, res);
  } else if (asInt(a) && b._type == types::_float) {
    res._bool = a.value._int > b.value._float;
    return data(types::_bool, res);
  } else if (a._type == types::_float && asInt(b)) {
    res._bool = a.value._float > b.value._int;
    return data(types::_bool, res);
  } else if (a._type == types::_float && b._type == types::_float) {
    res._bool = a.value._float > b.value._float;
    return data(types::_bool, res);
  } else {
    return data();
  }
}

data gt(data a, data b) {
  type_value res;
  if (asInt(a) && asInt(b)) {
    res._bool = a.value._int > b.value._int;
    return data(types::_bool, res);
  } else if (asInt(a) && b._type == types::_float) {
    res._bool = a.value._int > b.value._float;
    return data(types::_bool, res);
  } else if (a._type == types::_float && asInt(b)) {
    res._bool = a.value._float > b.value._int;
    return data(types::_bool, res);
  } else if (a._type == types::_float && b._type == types::_float) {
    res._bool = a.value._float > b.value._float;
    return data(types::_bool, res);
  } else {
    return data();
  }
}

data ee(data a, data b) {
  type_value res;
  if (asInt(a) && asInt(b)) {
    res._bool = a.value._int == b.value._int;
    return data(types::_bool, res);
  } else if (asInt(a) && b._type == types::_float) {
    res._bool = a.value._int == b.value._float;
    return data(types::_bool, res);
  } else if (a._type == types::_float && asInt(b)) {
    res._bool = a.value._float == b.value._int;
    return data(types::_bool, res);
  } else if (a._type == types::_float && b._type == types::_float) {
    res._bool = a.value._float == b.value._float;
    return data(types::_bool, res);
  } else {
    return data();
  }
}

data ne(data a, data b) {
  type_value res;
  if (asInt(a) && asInt(b)) {
    res._bool = a.value._int != b.value._int;
    return data(types::_bool, res);
  } else if (asInt(a) && b._type == types::_float) {
    res._bool = a.value._int != b.value._float;
    return data(types::_bool, res);
  } else if (a._type == types::_float && asInt(b)) {
    res._bool = a.value._float != b.value._int;
    return data(types::_bool, res);
  } else if (a._type == types::_float && b._type == types::_float) {
    res._bool = a.value._float != b.value._float;
    return data(types::_bool, res);
  } else {
    return data();
  }
}

#endif