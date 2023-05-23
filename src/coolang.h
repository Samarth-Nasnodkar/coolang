#include "includes.h"
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "interpreter/interpreter.h"

class Coolang {
Interpreter interpreter;
public:
  Coolang() {
    interpreter = Interpreter();
  }

  void run(std::string input) {
    Lexer lexer(input);
    auto res = lexer.run();
    if (res.second.has_error()) {
     std::cout << res.second.to_string() << std::endl;
     return;
    }

    // for( auto &token : res.first) {
    //   std::cout << token.to_string() << " ";
    // }
    // std::cout << std::endl;

    Parser parser(res.first);
    auto parseRes = parser.parse();
    if (parseRes.second.has_error()) {
      std::cout << parseRes.second.to_string() << std::endl;
    }

    // printNode(parseRes.first);
    // std::cout << std::endl;
    
    auto interpretRes = interpreter.visit(parseRes.first);

    if (interpretRes.get_error().has_error()) {
      std::cout << interpretRes.get_error().to_string() << std::endl;
    } else {
      std::cout << interpretRes.get_value().to_string() << std::endl;
    }
  }
};