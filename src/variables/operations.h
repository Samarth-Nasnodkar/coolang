#include "../includes.h"
#include "operators.h"
#include "data.h"

#include "int_ops/int_ops.h"
#include "float_ops/float_ops.h"
#include "str_ops/str_ops.h"
#include "list_ops/list_ops.h"

#ifndef OPERATIONS_H
#define OPERATIONS_H

bool asInt(data a) {
  return a._type == _int || a._type == _bool;
}

types getType(data a, data b) {
  if (a._type == b._type)
    return a._type;
  else if (a._type == types::_float || b._type == types::_float)
    return types::_float;
  else
    return a._type != types::_null ? a._type : b._type;
}

std::pair<data, Error> add(data a, data b) {
  type_value res;
  if (a._type == types::_int) {
    return int_add(a, b);
  } else if (a._type == types::_float) {
    return float_add(a, b);
  } else if (a._type == types::_string) {
    return str_add(a, b);
  } else if (a._type == types::_list_type) {
    return list_add(a, b);
  } else {
    return {data(), Error("Invalid operands", "Invalid operands for '+'")};
  }
}

std::pair<data, Error> sub(data a, data b) {
  type_value res;
  if (a._type == types::_int) {
    return int_sub(a, b);
  } else if (a._type == types::_float) {
    return float_sub(a, b);
  } else if (a._type == types::_string) {
    return str_sub(a, b);
  } else if (a._type == types::_list_type) {
    return list_sub(a, b);
  } else {
    return {data(), Error("Invalid operands", "Invalid operands for '-'")};
  }
}

std::pair<data, Error> mul(data a, data b) {
  type_value res;
  if (a._type == types::_int) {
    return int_mul(a, b);
  } else if (a._type == types::_float) {
    return float_mul(a, b);
  } else if (a._type == types::_string) {
    return str_mul(a, b);
  } else if (a._type == types::_list_type) {
    return list_mul(a, b);
  } else {
    return {data(), Error("Invalid operands", "Invalid operands for '*'")};
  }
}

std::pair<data, Error> div(data a, data b) {
  type_value res;
  if (a._type == types::_int) {
    return int_div(a, b);
  } else if (a._type == types::_float) {
    return float_div(a, b);
  } else if (a._type == types::_string) {
    return str_div(a, b);
  } else if (a._type == types::_list_type) {
    return list_div(a, b);
  } else {
    return {data(), Error("Invalid operands", "Invalid operands for '/'")};
  }
}

std::pair<data, Error> mod(data a, data b) {
  type_value res;
  if (a._type == types::_int) {
    return int_mod(a, b);
  } else if (a._type == types::_float) {
    return float_mod(a, b);
  } else if (a._type == types::_string) {
    return str_mod(a, b);
  } else if (a._type == types::_list_type) {
    return list_mul(a, b);
  } else {
    return {data(), Error("Invalid operands", "Invalid operands for '%'")};
  }
}

std::pair<data, Error> pow(data a, data b) {
  type_value res;
  if (a._type == types::_int) {
    return int_pow(a, b);
  } else if (a._type == types::_float) {
    return float_pow(a, b);
  } else if (a._type == types::_string) {
    return str_pow(a, b);
  } else if (a._type == types::_list_type) {
    return list_pow(a, b);
  } else {
    return {data(), Error("Invalid operands", "Invalid operands for '^'")};
  }
}

std::pair<data, Error> neg(data a) {
  type_value res;
  if (asInt(a)) {
    res._int = -a.value._int;
    return {data(types::_int, res), Error()};
  } else if (a._type == types::_float) {
    res._float = -a.value._float;
    return {data(types::_float, res), Error()};
  } else {
    return {data(), Error("Invalid Operand", "Invalid Operand around '-'")};
  }
}

std::pair<data, Error> logic_not(data a) {
  type_value res;
  // if (asInt(a)) {
  //   res._bool = !a.value._bool;
  //   return {types::_bool, res};
  // } else if (a._type == types::_float) {
  //   res._bool = !a.value._float;
  //   return {types::_bool, res};
  // }
  res._bool = !a.value._int;
  return {{types::_bool, res}, Error()};
}

std::pair<data, Error> logic_and(data a, data b) {
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
  return {data(types::_bool, res), Error()};
}

std::pair<data, Error> logic_or(data a, data b) {
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
  return {data(types::_bool, res), Error()};
}

std::pair<data, Error> le(data a, data b) {
  type_value res;
  if (a._type == types::_int) {
    auto r = int_cmp(a, b);
    if (r.second.has_error()) return {data(), r.second};
    res._bool = r.first.value._int <= 0;
  }
  else if (a._type == types::_float) {
    auto r = float_cmp(a, b);
    if (r.second.has_error()) return {data(), r.second};
    res._bool = r.first.value._int <= 0;
  }
  else if (a._type == types::_string) {
    auto r = str_cmp(a, b);
    if (r.second.has_error()) return {data(), r.second};
    res._bool = r.first.value._int <= 0;
  }
  else if (a._type == types::_list_type) {
    return list_cmp(a, b);
  } 
  return {data(types::_bool, res), Error()};
}

std::pair<data, Error> lt(data a, data b) {
  type_value res;
  if (a._type == types::_int) {
    auto r = int_cmp(a, b);
    if (r.second.has_error()) return {data(), r.second};
    res._bool = r.first.value._int < 0;
  }
  else if (a._type == types::_float) {
    auto r = float_cmp(a, b);
    if (r.second.has_error()) return {data(), r.second};
    res._bool = r.first.value._int < 0;
  }
  else if (a._type == types::_string) {
    auto r = str_cmp(a, b);
    if (r.second.has_error()) return {data(), r.second};
    res._bool = r.first.value._int < 0;
  }
  else if (a._type == types::_list_type) {
    return list_cmp(a, b);
  } 
  return {data(types::_bool, res), Error()};
}

std::pair<data, Error> ge(data a, data b) {
  type_value res;
  if (a._type == types::_int) {
    auto r = int_cmp(a, b);
    if (r.second.has_error()) return {data(), r.second};
    res._bool = r.first.value._int >= 0;
  }
  else if (a._type == types::_float) {
    auto r = float_cmp(a, b);
    if (r.second.has_error()) return {data(), r.second};
    res._bool = r.first.value._int >= 0;
  }
  else if (a._type == types::_string) {
    auto r = str_cmp(a, b);
    if (r.second.has_error()) return {data(), r.second};
    res._bool = r.first.value._int >= 0;
  }
  else if (a._type == types::_list_type) {
    return list_cmp(a, b);
  } 
  return {data(types::_bool, res), Error()};
}

std::pair<data, Error> gt(data a, data b) {
  type_value res;
  if (a._type == types::_int) {
    auto r = int_cmp(a, b);
    if (r.second.has_error()) return {data(), r.second};
    res._bool = r.first.value._int > 0;
  }
  else if (a._type == types::_float) {
    auto r = float_cmp(a, b);
    if (r.second.has_error()) return {data(), r.second};
    res._bool = r.first.value._int > 0;
  }
  else if (a._type == types::_string) {
    auto r = str_cmp(a, b);
    if (r.second.has_error()) return {data(), r.second};
    res._bool = r.first.value._int > 0;
  }
  else if (a._type == types::_list_type) {
    return list_cmp(a, b);
  } 
  return {data(types::_bool, res), Error()};
}

std::pair<data, Error> ee(data a, data b) {
  type_value res;
  if (a._type == types::_int) {
    auto r = int_cmp(a, b);
    if (r.second.has_error()) return {data(), r.second};
    res._bool = r.first.value._int == 0;
  }
  else if (a._type == types::_float) {
    auto r = float_cmp(a, b);
    if (r.second.has_error()) return {data(), r.second};
    res._bool = r.first.value._int == 0;
  }
  else if (a._type == types::_string) {
    auto r = str_cmp(a, b);
    if (r.second.has_error()) return {data(), r.second};
    res._bool = r.first.value._int == 0;
  }
  else if (a._type == types::_list_type) {
    return list_cmp(a, b);
  } 
  return {data(types::_bool, res), Error()};
}

std::pair<data, Error> ne(data a, data b) {
  type_value res;
  if (a._type == types::_int) {
    auto r = int_cmp(a, b);
    if (r.second.has_error()) return {data(), r.second};
    res._bool = r.first.value._int != 0;
  }
  else if (a._type == types::_float) {
    auto r = float_cmp(a, b);
    if (r.second.has_error()) return {data(), r.second};
    res._bool = r.first.value._int != 0;
  }
  else if (a._type == types::_string) {
    auto r = str_cmp(a, b);
    if (r.second.has_error()) return {data(), r.second};
    res._bool = r.first.value._int != 0;
  }
  else if (a._type == types::_list_type) {
    return list_cmp(a, b);
  } 
  return {data(types::_bool, res), Error()};
}

#endif