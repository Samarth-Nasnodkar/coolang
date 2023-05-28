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

  void run(std::string fileName) {
    std::fstream file;
    file.open(fileName, std::ios::in);
    if (!file.is_open()) {
      std::cout << "File not found" << std::endl;
      return;
    }
    std::string content;
    std::string line;
    while (std::getline(file, line)) {
      content += line + "\n";
    }
    Lexer lexer(content);
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

    // for( auto &r : parseRes.first) {
    //   printNode(r);
    // }
    // std::cout << std::endl;

    // printNode(parseRes.first);
    // std::cout << std::endl;
    
    for (auto &node : parseRes.first) {
      auto interpretRes = interpreter.visit(node);
      if (interpretRes.get_error().has_error()) {
      } else {
          if (!interpretRes.is_empty()) 
            std::cout << interpretRes.get_value().to_string() << std::endl;
      }
    }

    // auto interpretRes = interpreter.visit(parseRes.first);

    // if (interpretRes.get_error().has_error()) {
    //   std::cout << interpretRes.get_error().to_string() << std::endl;
    // } else {
    //   std::cout << interpretRes.get_value().to_string() << std::endl;
    // }
  }
};