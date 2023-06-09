#include "../includes.h"
#include "../variables/data.h"

#ifndef PRIMITIVE_FUNC_H
#define PRIMITIVE_FUNC_H

// __runtime_var print(std::pair<runtime_type, void*> a[], int n) {
//   for(int i = 0; i < n; ++i) {
//     // if (a[i].first != __var) {
//     //   std::cout << "TypeError: print() only accepts variables" << std::endl;
//     //   exit(1);
//     // }
//     if (a[i].first == __object) {
//       auto __obj = (obj *)a[i].second;
//       auto __obj_def = __obj->_def;
//       if (__obj_def.find("to_string") != __obj_def.end()) {
//         auto __str = __obj_def["to_string"];
//         auto __str_res = __str->visit();
//         std::cout << ((data *)__str_res.second)->to_string();
//       } else {
//         std::cout << "TypeError: print() only accepts variables" << std::endl;
//         exit(1);
//       }
//     }
//     if (i == n - 1)
//       std::cout << ((data *)a[i].second)->to_string();
//     else 
//       std::cout << ((data *)a[i].second)->to_string() << " ";
//   }
//   return {__var, new data{_null, type_value()}};
// }

// __runtime_var println(std::pair<runtime_type, void*> a[], int n) {
//   for(int i = 0; i < n; ++i) {
//     if (a[i].first != __var) {
//       std::cout << "TypeError: print() only accepts variables" << std::endl;
//       exit(1);
//     }
//     std::cout << ((data *)a[i].second)->to_string() << " ";
//   }
//   std::cout << std::endl;
//   return  {__var, new data{_null, type_value()}};
// }

// __runtime_var len(__runtime_var a) {
//   if (a.first != __var) {
//     std::cout << "TypeError: len() only accepts variables" << std::endl;
//     exit(1);
//   }
//   data _a = *((data *)a.second);
//   if (_a._type == types::_string) {
//     int _len = strlen(_a.value._string);
//     type_value v;
//     v._int = _len;
//     return {__var, new data(types::_int, v)};
//   } else if (_a._type == types::_list_type) {
//     int _len = _a.value._list->size;
//     type_value v;
//     v._int = _len;
//     return {__var, new data(types::_int, v)};
//   } else {
//     std::cout << "TypeError: len() only accepts strings and lists" << std::endl;
//     exit(1);
//   }
// }

// __runtime_var type_of(__runtime_var a) {
//   type_value v;
//   std::string _type_str;
//   if (a.first == __var) {
//     data _a = *((data *)a.second);
//     _type_str = types_str[_a._type];
//     v._string = new char[_type_str.length() + 1];
//   } else if (a.first == __object) {
//     obj _a = *((obj *)a.second);
//     _type_str = _a._type;
//     v._string = new char[_type_str.length() + 1];
//   } else if (a.first == __func) {
//     _type_str = "function.__" + ((node *)a.second)->token.get_name() + "__";
//     v._string = new char[_type_str.length() + 1];
//   } else {
//     _type_str = "undefined";
//   }
//   strncpy(v._string, _type_str.c_str(), _type_str.length() + 1);
//   return {__var, new data(types::_string, v)};
// }

#endif