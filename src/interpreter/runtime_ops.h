#include "runtime_type.h"
#include "../variables/operations.h"

#ifndef RUNTIME_OPS_H
#define RUNTIME_OPS_H

std::pair<__runtime_var, Error> binaryAdd(__runtime_var a, __runtime_var b) {
  if (a.first == __var && b.first == __var) {
    auto __res = add(*((data *)a.second), *((data *)b.second));
    if (__res.second.has_error()) return {{__ne, nullptr}, __res.second};
    return {{__var, new data{__res.first}}, Error()};
  } else if (a.first == __var && ((data *)a.second)->_type == _list_type) {
    return {{__ne, nullptr}, Error("Type Error", "Cannot use operator '+' on function")};
  } else if (a.first == __func || b.first == __func) {
    return {{__ne, nullptr}, Error("Type Error", "Cannot use operator '+' on function")};
  } else {
    return {{__ne, nullptr}, Error()};
  }
}
std::pair<__runtime_var, Error> binarySub(__runtime_var a, __runtime_var b) {
  if (a.first == __var && b.first == __var) {
    auto __res = sub(*((data *)a.second), *((data *)b.second));
    if (__res.second.has_error()) return {{__ne, nullptr}, __res.second};
    return {{__var, new data{__res.first}}, Error()};
  } else if (a.first == __func || b.first == __func) {
    return {{__ne, nullptr}, Error("Type Error", "Cannot use operator '-' on function")};
  } else {
    return {{__ne, nullptr}, Error()};
  }
}
std::pair<__runtime_var, Error> binaryMul(__runtime_var a, __runtime_var b) {
  if (a.first == __var && b.first == __var) {
    auto __res = mul(*((data *)a.second), *((data *)b.second));
    if (__res.second.has_error()) return {{__ne, nullptr}, __res.second};
    return {{__var, new data{__res.first}}, Error()};
  } else if (a.first == __func || b.first == __func) {
    return {{__ne, nullptr}, Error("Type Error", "Cannot use operator '*' on function")};
  } else {
    return {{__ne, nullptr}, Error()};
  }
}
std::pair<__runtime_var, Error> binaryDiv(__runtime_var a, __runtime_var b) {
  if (a.first == __var && b.first == __var) {
    auto __res = div(*((data *)a.second), *((data *)b.second));
    if (__res.second.has_error()) return {{__ne, nullptr}, __res.second};
    return {{__var, new data{__res.first}}, Error()};
  } else if (a.first == __func || b.first == __func) {
    return {{__ne, nullptr}, Error("Type Error", "Cannot use operator '+' on function")};
  } else {
    return {{__ne, nullptr}, Error()};
  }
}
std::pair<__runtime_var, Error> binaryMod(__runtime_var a, __runtime_var b) {
  if (a.first == __var && b.first == __var) {
    auto __res = mod(*((data *)a.second), *((data *)b.second));
    if (__res.second.has_error()) return {{__ne, nullptr}, __res.second};
    return {{__var, new data{__res.first}}, Error()};
  } else if (a.first == __func || b.first == __func) {
    return {{__ne, nullptr}, Error("Type Error", "Cannot use operator '+' on function")};
  } else {
    return {{__ne, nullptr}, Error()};
  }
}
std::pair<__runtime_var, Error> binaryPow(__runtime_var a, __runtime_var b) {
  if (a.first == __var && b.first == __var) {
    auto __res = pow(*((data *)a.second), *((data *)b.second));
    if (__res.second.has_error()) return {{__ne, nullptr}, __res.second};
    return {{__var, new data{__res.first}}, Error()};
  } else if (a.first == __func || b.first == __func) {
    return {{__ne, nullptr}, Error("Type Error", "Cannot use operator '+' on function")};
  } else {
    return {{__ne, nullptr}, Error()};
  }
}
std::pair<__runtime_var, Error> unaryNeg(__runtime_var a) {
  if (a.first == __var) {
    auto __res = neg(*((data *)a.second));
    if (__res.second.has_error()) return {{__ne, nullptr}, __res.second};
    return {{__var, new data{__res.first}}, Error()};
  } else if (a.first == __func) {
    return {{__ne, nullptr}, Error("Type Error", "Cannot use operator '-' on function")};
  } else {
    return {{__ne, nullptr}, Error()};
  }
}
std::pair<__runtime_var, Error> unaryNot(__runtime_var a) {
  if (a.first == __var) {
    auto __res = logic_not(*((data *)a.second));
    if (__res.second.has_error()) return {{__ne, nullptr}, __res.second};
    return {{__var, new data{__res.first}}, Error()};
  } else if (a.first == __func) {
    return {{__ne, nullptr}, Error("Type Error", "Cannot use operator '!' on function")};
  } else {
    return {{__ne, nullptr}, Error()};
  }
}
std::pair<__runtime_var, Error> binaryAnd(__runtime_var a, __runtime_var b) {
  if (a.first == __var && b.first == __var) {
    auto __res = logic_and(*((data *)a.second), *((data *)b.second));
    if (__res.second.has_error()) return {{__ne, nullptr}, __res.second};
    return {{__var, new data{__res.first}}, Error()};
  } else if (a.first == __func || b.first == __func) {
    return {{__ne, nullptr}, Error("Type Error", "Cannot use operator 'and' on function")};
  } else {
    return {{__ne, nullptr}, Error()};
  }
}
std::pair<__runtime_var, Error> binaryOr(__runtime_var a, __runtime_var b) {
  if (a.first == __var && b.first == __var) {
    auto __res = logic_or(*((data *)a.second), *((data *)b.second));
    if (__res.second.has_error()) return {{__ne, nullptr}, __res.second};
    return {{__var, new data{__res.first}}, Error()};
  } else if (a.first == __func || b.first == __func) {
    return {{__ne, nullptr}, Error("Type Error", "Cannot use operator 'or' on function")};
  } else {
    return {{__ne, nullptr}, Error()};
  }
}
std::pair<__runtime_var, Error> binaryLe(__runtime_var a, __runtime_var b) {
  if (a.first == __var && b.first == __var) {
    auto __res = le(*((data *)a.second), *((data *)b.second));
    if (__res.second.has_error()) return {{__ne, nullptr}, __res.second};
    return {{__var, new data{__res.first}}, Error()};
  } else if (a.first == __func || b.first == __func) {
    return {{__ne, nullptr}, Error("Type Error", "Cannot use operator '<=' on function")};
  } else {
    return {{__ne, nullptr}, Error()};
  }
}
std::pair<__runtime_var, Error> binaryLt(__runtime_var a, __runtime_var b) {
  if (a.first == __var && b.first == __var) {
    auto __res = lt(*((data *)a.second), *((data *)b.second));
    if (__res.second.has_error()) return {{__ne, nullptr}, __res.second};
    return {{__var, new data{__res.first}}, Error()};
  } else if (a.first == __func || b.first == __func) {
    return {{__ne, nullptr}, Error("Type Error", "Cannot use operator '<' on function")};
  } else {
    return {{__ne, nullptr}, Error()};
  }
}
std::pair<__runtime_var, Error> binaryGe(__runtime_var a, __runtime_var b) {
  if (a.first == __var && b.first == __var) {
    auto __res = ge(*((data *)a.second), *((data *)b.second));
    if (__res.second.has_error()) return {{__ne, nullptr}, __res.second};
    return {{__var, new data{__res.first}}, Error()};
  } else if (a.first == __func || b.first == __func) {
    return {{__ne, nullptr}, Error("Type Error", "Cannot use operator '>=' on function")};
  } else {
    return {{__ne, nullptr}, Error()};
  }
}
std::pair<__runtime_var, Error> binaryGt(__runtime_var a, __runtime_var b) {
  if (a.first == __var && b.first == __var) {
    auto __res = gt(*((data *)a.second), *((data *)b.second));
    if (__res.second.has_error()) return {{__ne, nullptr}, __res.second};
    return {{__var, new data{__res.first}}, Error()};
  } else if (a.first == __func || b.first == __func) {
    return {{__ne, nullptr}, Error("Type Error", "Cannot use operator '>' on function")};
  } else {
    return {{__ne, nullptr}, Error()};
  }
}
std::pair<__runtime_var, Error> binaryEe(__runtime_var a, __runtime_var b) {
  if (a.first == __var && b.first == __var) {
    auto __res = ee(*((data *)a.second), *((data *)b.second));
    if (__res.second.has_error()) return {{__ne, nullptr}, __res.second};
    return {{__var, new data{__res.first}}, Error()};
  } else if (a.first == __func || b.first == __func) {
    return {{__ne, nullptr}, Error("Type Error", "Cannot use operator '==' on function")};
  } else {
    return {{__ne, nullptr}, Error()};
  }
}
std::pair<__runtime_var, Error> binaryNe(__runtime_var a, __runtime_var b) {
  if (a.first == __var && b.first == __var) {
    auto __res = ne(*((data *)a.second), *((data *)b.second));
    if (__res.second.has_error()) return {{__ne, nullptr}, __res.second};
    return {{__var, new data{__res.first}}, Error()};
  } else if (a.first == __func || b.first == __func) {
    return {{__ne, nullptr}, Error("Type Error", "Cannot use operator '!=' on function")};
  } else {
    return {{__ne, nullptr}, Error()};
  }
}

#endif