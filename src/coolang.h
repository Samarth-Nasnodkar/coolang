#include "includes.h"
#include "lexer/lexer.h"
#include "parser/parser.h"

class Coolang {
public:
  Coolang() {
  }

  void run(std::string input) {
    Lexer lexer(input);
    auto res = lexer.run();
    if (res.second.has_error()) {
     std::cout << res.second.to_string() << std::endl;
     return;
    }

    Parser parser(res.first);
    auto parseRes = parser.parse();
    if (parseRes.second.has_error()) {
      std::cout << parseRes.second.to_string() << std::endl;
    }
    for(auto &node : parseRes.first) {
      printNode(node);
      std::cout << std::endl;
    }
  }
};