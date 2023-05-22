#include "../includes.h"
#include "error_status.h"

#ifndef ERROR_H
#define ERROR_H

class Error {
  std::string name;
  std::string message;
  error_status status;
public:
  Error() {
    status = error_status::OK;
  }
  
  Error(std::string _name, std::string _message) {
    name = std::move(_name);
    message = std::move(_message);
    status = error_status::ERROR;
  }

  error_status get_status() {
    return status;
  }

  bool has_error() {
    return status == error_status::ERROR;
  }

  std::string to_string() {
    return name + " : " + message;
  }
};

#endif // ERROR_H