#include "../includes.h"

#ifndef TYPE_VALUE_H
#define TYPE_VALUE_H

struct _llist {
  void *_data;
  int size;
};

union type_value {
  long int _int;
  double _float;
  char _char;
  bool _bool;
  char *_string;
  _llist *_list;

  type_value() {
    _int = 0;
  }
};

#endif // TYPE_VALUE_H
