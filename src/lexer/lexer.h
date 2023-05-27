#include "cursor_position.h"
#include "token.h"
#include "../variables/operators.h"
#include "../error/error.h"
#include "../variables/shot_hand_op.h"

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

  bool isKeyword(std::string cmp) {
    for(int i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) {
      if (keywords[i] == cmp) return true;
    }
    return false;
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

  bool isAlpha() {
    return (currentChar >= 65 && currentChar <= 90) || (currentChar >= 97 && currentChar <= 122);
  }

  bool isAlphaNumeric() {
    return isAlpha() || isNumber();
  }

  std::pair<std::vector<Token>, Error> run() {
    std::vector<Token> tokens;

    while (currentChar != '\0') {
      if (currentChar == ' ' || currentChar == '\t') {
        advance();
        continue;
      }
      if (currentChar == '\n') {
        while (currentChar == '\n') advance();
        tokens.emplace_back(Token(std::move(KEY_NEWLINE), currentPos, currentPos));
        // advance();
        continue;
      }
      if (currentChar == '=' || currentChar == '!') {
        auto res = makeEq();
        if (!res.has_data() && res.get_name().length() == 0) {
          tokens.clear();
          return {tokens, Error("Invalid Syntax Error", "Expected '='")};
        }
        tokens.emplace_back(res);
        continue;
      }
      if (currentChar == '<') {
        tokens.emplace_back(makeLTE());
        continue;
      }
      if (currentChar == '>') {
        tokens.emplace_back(makeGTE());
        continue;
      }
      int op_index = isOperator();
      if (op_index != -1) {
        CursorPosition startPos = currentPos;
        advance();
        bool _short_hand = currentChar == '=' && operators[op_index].binary;
        if (_short_hand) 
          advance();
        CursorPosition endPos = currentPos;
        tokens.emplace_back(Token(std::move(_short_hand ? short_hand_ops[op_index].name : operators[op_index].name), startPos, endPos));
        continue;
      }
      if (isNumber()) {
        tokens.emplace_back(makeNumberToken());
        continue;
      }
      if (isAlpha()) {
        tokens.emplace_back(makeIdentifierToken());
        continue;
      }
      tokens.clear();
      std::string msg = "Invalid Character '";
      msg += currentChar;
      msg += '\'';
      return std::make_pair(tokens, Error("Invalid Character Error", msg));
    }
    CursorPosition endPos = currentPos;
    tokens.emplace_back(Token(KEY_EOF, currentPos, endPos));
    return std::make_pair(tokens, Error());
  }

  Token makeLTE() {
    CursorPosition startpos = currentPos;
    advance();

    if (currentChar == '=') {
      advance();
      return Token(KEY_LE, startpos, currentPos);
    }

    return Token(KEY_LT, startpos, currentPos);
  }

  Token makeGTE() {
    CursorPosition startpos = currentPos;
    advance();

    if (currentChar == '=') {
      advance();
      return Token(KEY_GE, startpos, currentPos);
    }

    return Token(KEY_GT, startpos, currentPos);
  }

  Token makeEq() {
    bool ne = (currentChar == '!');
    CursorPosition startpos = currentPos;
    advance();

    if (currentChar == '=') {
      advance();
      return Token(ne ? KEY_NE : KEY_EE, startpos, currentPos);
    }
    if (!ne)
      return Token(KEY_EQ, startpos, currentPos);
    return Token();
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
      return {KEY_FLOAT, {types::_float, _value}, currentPos, endPos};
    } else {
      _value._int = stoi(num_str);
      return {KEY_INT, {types::_int, _value}, currentPos, endPos};
    }
  }

  Token makeIdentifierToken() {
    CursorPosition startPos = currentPos;
    std::string id_str = std::string(1, currentChar);
    advance();

    while (isAlphaNumeric()) {
      id_str += currentChar;
      advance();
    }
    if (isKeyword(id_str)) {
      type_value v;
      if (id_str == KEY_TRUE) {
        v._bool = true;
        return {id_str, {types::_bool, v}, startPos, currentPos};
      } else if (id_str == KEY_FALSE) {
        v._bool = false;
        return {id_str, {types::_bool, v}, startPos, currentPos};
      } else if (id_str == KEY_NULL) {
        v._int = 0;
        return {id_str, {types::_null, v}, startPos, currentPos};
      }
      return {id_str, {types::_keyword, type_value()}, startPos, currentPos};
    } else {
      return {id_str, {types::_id, type_value()}, startPos, currentPos};
    }
  }
};

#endif // LEXER_H