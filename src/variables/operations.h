#include "operators.h"
#include "data.h"

#ifndef OPERATIONS_H
#define OPERATIONS_H

data add(data a, data b) {
  type_value res;
  if (a._type == types::_int && b._type == types::_int) {
    res._int = a.value._int + b.value._int;
    return data(types::_int, res);
  } else if (a._type == types::_int && b._type == types::_float) {
    res._float = a.value._int + b.value._float;
    return data(types::_float, res);
  } else if (a._type == types::_float && b._type == types::_int) {
    res._float = a.value._float + b.value._int;
    return data(types::_float, res);
  } else if (a._type == types::_float && b._type == types::_float) {
    res._float = a.value._float + b.value._float;
    return data(types::_float, res);
  } else {
    return data();
  }
}

data sub(data a, data b) {
  type_value res;
  if (a._type == types::_int && b._type == types::_int) {
    res._int = a.value._int - b.value._int;
    return data(types::_int, res);
  } else if (a._type == types::_int && b._type == types::_float) {
    res._float = a.value._int - b.value._float;
    return data(types::_float, res);
  } else if (a._type == types::_float && b._type == types::_int) {
    res._float = a.value._float - b.value._int;
    return data(types::_float, res);
  } else if (a._type == types::_float && b._type == types::_float) {
    res._float = a.value._float - b.value._float;
    return data(types::_float, res);
  } else {
    return data();
  }
}

data mul(data a, data b) {
  type_value res;
  if (a._type == types::_int && b._type == types::_int) {
    res._int = a.value._int * b.value._int;
    return data(types::_int, res);
  } else if (a._type == types::_int && b._type == types::_float) {
    res._float = a.value._int * b.value._float;
    return data(types::_float, res);
  } else if (a._type == types::_float && b._type == types::_int) {
    res._float = a.value._float * b.value._int;
    return data(types::_float, res);
  } else if (a._type == types::_float && b._type == types::_float) {
    res._float = a.value._float * b.value._float;
    return data(types::_float, res);
  } else {
    return data();
  }
}

data div(data a, data b) {
  type_value res;
  if (a._type == types::_int && b._type == types::_int) {
    res._int = a.value._int / b.value._int;
    return data(types::_int, res);
  } else if (a._type == types::_int && b._type == types::_float) {
    res._float = a.value._int / b.value._float;
    return data(types::_float, res);
  } else if (a._type == types::_float && b._type == types::_int) {
    res._float = a.value._float / b.value._int;
    return data(types::_float, res);
  } else if (a._type == types::_float && b._type == types::_float) {
    res._float = a.value._float / b.value._float;
    return data(types::_float, res);
  } else {
    return data();
  }
}

data mod(data a, data b) {
  type_value res;
  if (a._type == types::_int && b._type == types::_int) {
    res._int = a.value._int % b.value._int;
    return data(types::_int, res);
  } else if (a._type == types::_int && b._type == types::_float) {
    res._float = fmod(a.value._int, b.value._float);
    return data(types::_float, res);
  } else if (a._type == types::_float && b._type == types::_int) {
    res._float = fmod(a.value._float, b.value._int);
    return data(types::_float, res);
  } else if (a._type == types::_float && b._type == types::_float) {
    res._float = fmod(a.value._float, b.value._float);
    return data(types::_float, res);
  } else {
    return data();
  }
}

data pow(data a, data b) {
  type_value res;
  if (a._type == types::_int && b._type == types::_int) {
    res._int = pow(a.value._int, b.value._int);
    return data(types::_int, res);
  } else if (a._type == types::_int && b._type == types::_float) {
    res._float = pow(a.value._int, b.value._float);
    return data(types::_float, res);
  } else if (a._type == types::_float && b._type == types::_int) {
    res._float = pow(a.value._float, b.value._int);
    return data(types::_float, res);
  } else if (a._type == types::_float && b._type == types::_float) {
    res._float = pow(a.value._float, b.value._float);
    return data(types::_float, res);
  } else {
    return data();
  }
}

data neg(data a) {
  type_value res;
  if (a._type == types::_int) {
    res._int = -a.value._int;
    return data(types::_int, res);
  } else if (a._type == types::_float) {
    res._float = -a.value._float;
    return data(types::_float, res);
  } else {
    return data();
  }
}

#endif