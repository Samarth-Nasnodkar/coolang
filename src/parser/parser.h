#include "../includes.h"
#include "../lexer/lexer.h"
#include "../lexer/token.h"
#include "../variables/cmp.h"
#include "node.h"

/*
atom : INT | FLOAT | IDENTIFIER
     : LPAREN expr RPAREN

PREC_0 : atom POW atom
       : atom

PREC_1 : PREC_0
       : PLUS PREC_1
       : MINUS PREC_1

PREC_2 : PREC_1 MUL PREC_1
       : PREC_1 DIV PREC_1
       : PREC_1 MOD PREC_1
       : PREC_1

PREC_3 : PREC_2 ADD PREC_2
        : PREC_2 SUB PREC_2
        : PREC_2

cond : PREC_3 cmp PREC_3
     : PREC_3

PREC_TOP : let IDENTIFIER EQ cond

code_block : { cond... }
           : cond EOL | EOF
*/

#ifndef PARSER_H
#define PARSER_H

class Parser {
std::vector<Token> tokens;
int currentTokenIndex;
Token currentToken;
public:
  Parser(std::vector<Token> _tokens) {
    tokens = _tokens;
    currentTokenIndex = -1;
    advance();
  }

  void advance() {
    currentTokenIndex++;
    if (currentTokenIndex < tokens.size()) {
      currentToken = tokens[currentTokenIndex];
    } else {
      currentToken = Token(KEY_EOF, CursorPosition(), CursorPosition());
    }
    // std::cout << currentToken.to_string() << std::endl;
  }

  bool isUnary() {
    return currentToken.get_name() == KEY_ADD || currentToken.get_name() == KEY_SUB || currentToken.get_name() == KEY_NOT;
  }

  bool isCmp() {
    for (auto cmp : cmps) {
      if (cmp.name == currentToken.get_name()) {
        return true;
      }
    }
    return false;
  }

  std::pair<std::vector<node *>, Error> parse() {
    std::vector<node *> res;
    while (currentToken.get_name() != KEY_EOF) {
      auto _res = parseCodeBlock();
      if (_res.second.has_error()) return std::make_pair(std::vector<node *>(), _res.second);
      res.push_back(_res.first);
      advance();
    }
    return std::make_pair(res, Error());
  }

  std::pair<node *, Error> parseAtom() {
    if (currentToken.get_value()._type == types::_null) {
      node *_node = new node{currentToken, node_type::_nullnode, nullptr, nullptr};
      advance();
      return std::make_pair(_node, Error());
    }
    if (currentToken.get_value()._type == types::_int || currentToken.get_value()._type == types::_float) {
      node *_node = new node{currentToken, node_type::_number, nullptr, nullptr};
      advance();
      return std::make_pair(_node, Error());
    } 

    if (currentToken.get_value()._type == types::_bool) {
      node *_node = new node{currentToken, node_type::_boolean, nullptr, nullptr};
      advance();
      return std::make_pair(_node, Error());
    }

    if (currentToken.get_name() == KEY_LPAREN) {
      advance();
      auto expr = parsePrec3();
      if (expr.second.has_error()) return std::make_pair(new node(), expr.second);
      if (currentToken.get_name() != KEY_RPAREN) {
        return std::make_pair(new node(), Error("Invalid Syntax Error", "Expected ')'"));
      }
      advance();
      return expr;
    }

    if (currentToken.get_value()._type == types::_id) {
      node *_node = new node{currentToken, node_type::_variable_access, nullptr, nullptr};
      advance();
      return std::make_pair(_node, Error());
    }

    std::cout << currentToken.to_string() << std::endl;

    return std::make_pair(new node(), Error("Invalid Syntax Error", "Expected Number"));
  }

  std::pair<node *, Error> parsePrec0() {
    auto left = parseAtom();
    if (left.second.has_error()) return std::make_pair(new node(), left.second);
    if (currentToken.get_name() == KEY_POW) {
      auto op = currentToken;
      advance();
      auto right = parseAtom();
      if (right.second.has_error()) return std::make_pair(new node(), right.second);
      return std::make_pair(new node{op, node_type::_binary_operator, left.first, right.first}, Error());
    }
    return left;
  }

  std::pair<node *, Error> parsePrec1() {
    if (isUnary()) {
      auto op = currentToken;
      advance();
      auto right = parsePrec1();
      if (right.second.has_error()) return std::make_pair(new node(), right.second);
      return std::make_pair(new node{op, node_type::_unary_operator, nullptr, right.first}, Error());
    }
    return parsePrec0();
  }

  std::pair<node *, Error> parsePrec2() {
    auto left = parsePrec1();
    if (left.second.has_error()) return std::make_pair(new node(), left.second);
    if (currentToken.get_name() == KEY_MUL || currentToken.get_name() == KEY_DIV || currentToken.get_name() == KEY_MOD) {
      auto op = currentToken;
      advance();
      auto right = parsePrec1();
      if (right.second.has_error()) return std::make_pair(new node(), right.second);
      return std::make_pair(new node{op, node_type::_binary_operator, left.first, right.first}, Error());
    }
    return left;
  }

  std::pair<node *, Error> parsePrec3() {
    auto left = parsePrec2();
    if (left.second.has_error()) return std::make_pair(new node(), left.second);
    if (currentToken.get_name() == KEY_ADD || currentToken.get_name() == KEY_SUB) {
      auto op = currentToken;
      advance();
      auto right = parsePrec2();
      if (right.second.has_error()) return std::make_pair(new node(), right.second);
      return std::make_pair(new node{op, node_type::_binary_operator, left.first, right.first}, Error());
    }
    return left;
  }

  std::pair<node *, Error> parseCond() {
    auto expr = parsePrec3();
    if (isCmp()) {
      auto op = currentToken;
      advance();
      auto expr2 = parsePrec3();
      if (expr2.second.has_error()) return std::make_pair(new node(), expr2.second);
      return std::make_pair(new node{op, node_type::_binary_operator, expr.first, expr2.first}, Error());
    }
    return expr;
  }

  std::pair<node *, Error> parsePrecTop() {
    if (currentToken.get_value()._type == types::_keyword && currentToken.get_name() == KEY_ASSIGN) {
      advance();
      if (currentToken.get_value()._type != types::_id) {
        return std::make_pair(new node(), Error("Invalid Syntax Error", "Expected Identifier"));
      }
      auto identifier = currentToken;
      advance();
      if (currentToken.get_name() != KEY_EQ) {
        return std::make_pair(new node(), Error("Invalid Syntax Error", "Expected '='"));
      }
      advance();
      auto expr = parseCond();
      if (expr.second.has_error()) return std::make_pair(new node(), expr.second);

      if (currentToken.get_name() != KEY_NEWLINE && currentToken.get_name() != KEY_EOF) {
        return std::make_pair(new node(), Error("Invalid Syntax Error", "Expected new line"));
      }

      return std::make_pair(new node{identifier, node_type::_variable_assign, expr.first, nullptr}, Error());
    }
    return parseCond();
  }

  std::pair<node *, Error> parseCodeBlock() {
    if (currentToken.get_name() == KEY_LBRACE) {
      advance();
      std::vector<node *> nodes;
      while (currentToken.get_name() != KEY_RBRACE) {
        if (currentToken.get_name() == KEY_NEWLINE) {
          advance();
          continue;
        }
        if (currentToken.get_name() == KEY_LBRACE) {
          auto code_block = parseCodeBlock();
          if (code_block.second.has_error()) return std::make_pair(new node(), code_block.second);
          nodes.push_back(code_block.first);
          continue;
        }
        auto expr = parsePrecTop();
        if (expr.second.has_error()) return std::make_pair(new node(), expr.second);
        nodes.push_back(expr.first);
      }
      advance();
      return std::make_pair(new node{Token(KEY_CODE_BLOCK, CursorPosition(), CursorPosition()), node_type::_code_block, nullptr, nullptr, nodes}, Error());
    }

    return parsePrecTop();
  }
};

#endif