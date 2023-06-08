#include "scope.h"
#include "runtime_type.h"

#ifndef GET_VAR_VALUE_H
#define GET_VAR_VALUE_H

__runtime_var getVarValue(scope &_scope, std::string name) {
  if (_scope.localScope.find(name) != _scope.localScope.end()) {
    return _scope.localScope[name];
  } else {
    return std::make_pair(__ne, nullptr);
  }
}

#endif