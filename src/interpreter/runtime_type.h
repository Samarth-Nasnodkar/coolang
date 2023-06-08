#include "../includes.h"

#ifndef RUNTIME_TYPE_H
#define RUNTIME_TYPE_H

enum runtime_type {
  __var,
  __object,
  __func,
  __ne,
};

#define __runtime_var std::pair<runtime_type, void *>

#endif