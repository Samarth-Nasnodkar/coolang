#ifndef TYPES_H
#define TYPES_H

enum types {
  _int,
  _float,
  _string,
  _null,
  _bool,
  _list_type,
  _keyword,
  _id,
};

std::string types_str[] = {
  "int",
  "float",
  "str",
  "null",
  "bool",
};

#endif // TYPES_H