#include "cursor_position.h"
#include "token.h"
#include "../variables/operators.h"
#include "../error/error.h"

#ifndef LEXER_H
#define LEXER_H

class Lexer {
  std::string text;
  CursorPosition currentPos;
  char currentChar;
public:
  Lexer(std::string _text) {
    text = _text;
    currentPos = CursorPosition();
    advance();
  }

  void advance() {
    currentPos.advance(currentChar);

    currentChar = currentPos.index == text.length() ? '\0' : text[currentPos.index];
  }

  int isOperator() {
    for(int i = 0; i < sizeof(operators) / sizeof(operators[0]); i++) {
      if (operators[i].op == currentChar) return i;
    }
    return -1;
  }

  bool isNumber() {
    return currentChar >= 48 && currentChar <= 57;
  }

  std::pair<std::vector<Token>, Error> run() {
    std::vector<Token> tokens;

    while (currentChar != '\0') {
      if (currentChar == ' ' || currentChar == '\t') {
        advance();
        continue;
      }
      int op_index = isOperator();
      if (op_index != -1) {
        CursorPosition endPos = currentPos;
        endPos.advance();
        tokens.emplace_back(Token(std::move(operators[op_index].name), currentPos, endPos));
        advance();
        continue;
      }
      if (isNumber()) {
        tokens.emplace_back(makeNumberToken());
        continue;
      }
      tokens.clear();
      std::string msg = "Invalid Character '";
      msg += currentChar;
      msg += '\'';
      return std::make_pair(tokens, Error("Invalid Character Error", msg));
    }
    CursorPosition endPos = currentPos;
    tokens.emplace_back(Token("EOF", currentPos, endPos));
    return std::make_pair(tokens, Error());
  }

  Token makeNumberToken() {
    int dotCount = 0;
    std::string num_str = std::string(1, currentChar);
    advance();

    while (isNumber() || currentChar == '.') {
      if (currentChar == '.') {
        if (dotCount == 1)
          break;
        dotCount++;
      }
      num_str += currentChar;
      advance();
    }
    type_value _value;
    CursorPosition endPos = currentPos;
    if (dotCount == 1) {
      _value._float = stof(num_str);
      return {"FLOAT", {types::_float, _value}, currentPos, endPos};
    } else {
      _value._int = stoi(num_str);
      return {"INT", {types::_int, _value}, currentPos, endPos};
    }
  }
};

#endif // LEXER_H